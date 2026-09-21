#include <algorithm>
#include <numbers>

#include "coordinates/transformation.hpp"
#include "core/constants.hpp"
#include "positioning/corrections/troposphere/saastamoinen.hpp"
#include "positioning/measurements/code_variance_model.hpp"
#include "positioning/measurements/linearization/pseudorange_linearizer.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

namespace {
std::optional<double>
calcElevationIfPossible(const Eigen::Vector3d &receiver_ecef,
                        const Eigen::Vector3d &satellite_ecef) {

  if (receiver_ecef.norm() == 0.0 || satellite_ecef.norm() == 0.0) {
    return std::nullopt;
  }

  return std::optional<double>(
      elevationFromEcef(receiver_ecef, satellite_ecef));
}

} // namespace

std::optional<LinearizedSystem>
LinearizePseudorangeEpoch(const PositioningEpoch &epoch,
                          const ReceiverState &receiver) {
  Eigen::Index row = 0;
  Eigen::Index rowCount = 0;
  for (const auto &measurement : epoch.measurements) {
    for (const auto &[signalId, observation] :
         measurement.observations.signals) {
      if (observation.CodeObservation)
        ++rowCount;
    }
  }

  LinearizedSystem linearizedSystem{
      .jacobian = Eigen::MatrixXd(rowCount, 4),
      .residuals = Eigen::VectorXd(rowCount),
      .variances_m2 = Eigen::VectorXd(rowCount),
  };

  const auto lla = receiver.position.toWgs84();

  for (const auto &measurement : epoch.measurements) {
    const auto &signals = measurement.observations.signals;
    if (std::none_of(signals.begin(), signals.end(), [](const auto &signal) {
          return signal.second.CodeObservation.has_value();
        })) {
      continue;
    }

    std::optional<double> optionalElvation =
        calcElevationIfPossible(receiver.position.vector(),
                                measurement.satelliteState.Position_E.vector());

    double elevation_rad = optionalElvation ? *optionalElvation : 0;

    troposphere::Model tropoModel = &troposphere::SaastamoinenChao;

    auto tropoDelay = tropoModel(
        {.latitude_rad = lla.latitude_deg * std::numbers::pi / 180.0,
         .height_m = lla.altitude_m, // TODO: undulation is missing here!
         .elevation_rad = elevation_rad});

    for (const auto &signal : measurement.observations.signals) {

      const auto &obs = signal.second;

      if (!obs.CodeObservation)
        continue;

      const auto tropoDelayTotal = tropoDelay ? tropoDelay->Total() : 0.0;
      const CorrectionContext context{.troposphere_m = tropoDelayTotal,
                                      .ionosphere_m = 0,
                                      .groupDelay_m = 0};

      const CodeObservation observation{.satellite =
                                            measurement.observations.satellite,
                                        .signalId = signal.first,
                                        .code = *obs.CodeObservation};

      PseudorangeModel model;
      const auto prediction = model.Evaluate(
          observation, receiver, measurement.satelliteState, context);

      linearizedSystem.residuals(row) =
          observation.code.pseudorange_m - prediction.predicted_m;

      CodeVarianceContext codeVarContext{
          .elevation_rad = optionalElvation,
          .cn0_dbhz = obs.SnrObservation
                          ? std::optional<double>{obs.SnrObservation->snr}
                          : std::nullopt};

      linearizedSystem.variances_m2(row) =
          CodeVarianceModel(observation, codeVarContext);

      // append jacobians
      for (Eigen::Index column = 0; column < 3; ++column) {
        linearizedSystem.jacobian(row, column) =
            prediction.d_predicted_d_receiver_position[column];
      }

      linearizedSystem.jacobian(row, 3) =
          prediction.d_predicted_d_receiver_clock_bias;

      ++row;
    }
  }

  // nonfinite
  if (!linearizedSystem.jacobian.allFinite() ||
      !linearizedSystem.residuals.allFinite() ||
      !linearizedSystem.variances_m2.allFinite() ||
      (linearizedSystem.variances_m2.array() <= 0.0).any()) {
    return std::nullopt;
  }

  return linearizedSystem;
}