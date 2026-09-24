#include <algorithm>
#include <numbers>

#include "coordinates/transformation.hpp"
#include "positioning/corrections/troposphere/saastamoinen.hpp"
#include "positioning/measurements/code_variance_model.hpp"
#include "positioning/measurements/prepare_code_epoch.hpp"

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

std::vector<PreparedCodeMeasurement> PrepareCodeEpoch(
    const PositioningEpoch &epoch, const ReceiverState &receiver) {
  std::vector<PreparedCodeMeasurement> prepared;
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

      CodeVarianceContext codeVarContext{
          .elevation_rad = optionalElvation,
          .cn0_dbhz = obs.SnrObservation
                          ? std::optional<double>{obs.SnrObservation->snr}
                          : std::nullopt};

      prepared.push_back({observation, measurement.satelliteState, context,
                          CodeVarianceModel(observation, codeVarContext)});

    }
  }
  return prepared;
}
