#include "positioning/measurements/linearization/pseudorange_linearizer.hpp"
#include "coordinates/transformation.hpp"
#include "positioning/measurements/code_variance_model.hpp"
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

bool LinearizeEpoch(const PositioningEpoch &epoch,
                    const ReceiverState &receiver,
                    const CorrectionContext &context,
                    LinearizedSystem &linearizedSystem) {
  Eigen::Index row = 0;

  for (const auto &measurement : epoch.measurements) {
    for (const auto &signal : measurement.observations.signals) {

      const auto obs = signal.second;

      if (!obs.CodeObservation)
        continue;

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
          .elevation_rad = calcElevationIfPossible(
              receiver.position.vector(),
              measurement.satelliteState.Position_E.vector()),
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
  return linearizedSystem.jacobian.allFinite() &&
         linearizedSystem.residuals.allFinite();
}