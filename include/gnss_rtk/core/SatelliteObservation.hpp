#pragma once

#include "gnss_rtk/core/satellite_id.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationBand.hpp"
#include "gnss_rtk/rinex/Observations.hpp"

#include <map>

struct SatelliteObservation
{
    SatelliteId satellite;

    std::map<ObservationBand, Rinex::Observation::Code> CodeObservations;
    std::map<ObservationBand, Rinex::Observation::CarrierPhase> PhaseObservations;
    std::map<ObservationBand, Rinex::Observation::Doppler> DopplerObservations;
    std::map<ObservationBand, Rinex::Observation::SignalStrength> SnrObservations;
};
