#pragma once

#include "core/satellite_id.hpp"
#include "rinex/RinexTypes/ObservationBand.hpp"
#include "rinex/Observations.hpp"

#include <compare>
#include <map>

#include "rinex/RinexTypes/ObservationAttribute.hpp"

// Signal identity within a satellite; observation type selects the map.
struct SignalId
{
    ObservationBand band{ObservationBand::Band_Unkown};
    ObservationAttribute attribute{ObservationAttribute::Channel_Unknown};

    auto operator<=>(const SignalId &) const = default;
};

struct SatelliteObservation
{
    SatelliteId satellite;

    std::map<SignalId, Rinex::Observation::Code> CodeObservations;
    std::map<SignalId, Rinex::Observation::CarrierPhase> PhaseObservations;
    std::map<SignalId, Rinex::Observation::Doppler> DopplerObservations;
    std::map<SignalId, Rinex::Observation::SignalStrength> SnrObservations;
};
