#include "positioning/providers/broadcast_epoch_provider.hpp"
#include "core/constants.hpp"
#include "navigation/ephemeris_traits.hpp"

#include <chrono>
#include <ranges>
#include <variant>

PositioningEpoch
BroadcastEpochProvider::GetEpoch(const ObservationEpoch &epoch) const {
  PositioningEpoch result{
      .receptionTime = epoch.time.Time(),
      .measurements = {},
  };

  const navigation::GnssTime receptionTime = epoch.time.Time();

  for (const SatelliteObservation &obs : epoch.satellites) {
    // find obs satellite in nav
    const auto navSatellite =
        std::ranges::find(this->_navigation.satellites, obs.satellite,
                          &SatelliteNavigation::satellite);

    // did not find any
    if (navSatellite == this->_navigation.satellites.end())
      continue;

    const auto signal =
        std::ranges::find_if(obs.signals, [](const auto &entry) {
          return entry.second.CodeObservation.has_value();
        });

    if (signal == obs.signals.end())
      continue;

    const double pseudorange = signal->second.CodeObservation->pseudorange_m;

    const auto signalTravelTime =
        navigation::Seconds{pseudorange / constants::SpeedOfLight__mDs};
    const auto transmissionTime = navigation::GnssTime{
        std::chrono::round<navigation::GnssClock::duration>(receptionTime -
                                                            signalTravelTime)};

    const NavigationMessage *message =
        navSatellite->FindMessage(transmissionTime);

    if (!message)
      continue;

    const SatelliteState state = std::visit(
        [&](const auto &nav) {
          return navigation::CalculateEphemeris(nav, transmissionTime);
        },
        *message);

    result.measurements.push_back(
        SatelliteMeasurement{.observations = obs,
                             .satelliteState = state,
                             .approximateTravelTime = signalTravelTime});
  }

  return result;
}
