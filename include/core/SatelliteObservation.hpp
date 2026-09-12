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

struct SignalObservation{
    std::optional<Rinex::Observation::Code> CodeObservation;
    std::optional<Rinex::Observation::CarrierPhase> PhaseObservation;
    std::optional<Rinex::Observation::Doppler> DopplerObservation;
    std::optional<Rinex::Observation::SignalStrength> SnrObservation;
};

struct SatelliteObservation
{
    SatelliteId satellite;
    std::map<SignalId, SignalObservation> signals;
};
