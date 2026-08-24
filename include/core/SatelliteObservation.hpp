#pragma once

#include "core/satellite_id.hpp"
#include "rinex/RinexTypes/ObservationBand.hpp"
#include "rinex/Observations.hpp"

#include <map>

struct SatelliteObservation
{
    SatelliteId satellite;

    std::map<ObservationBand, Rinex::Observation::Code> CodeObservations;
    std::map<ObservationBand, Rinex::Observation::CarrierPhase> PhaseObservations;
    std::map<ObservationBand, Rinex::Observation::Doppler> DopplerObservations;
    std::map<ObservationBand, Rinex::Observation::SignalStrength> SnrObservations;
};
