#pragma once

#include "gnss_rtk/core/abstract_satellite.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationBand.hpp"
#include "gnss_rtk/rinex/Observations.hpp"

#include <map>

struct SatelliteObservation
{
    AbstractSatellite satellite;

    std::map<ObservationBand, Rinex::Observation::Code> CodeObservations;
    std::map<ObservationBand, Rinex::Observation::Phase> PhaseObservations;
    std::map<ObservationBand, Rinex::Observation::Doppler> DopplerObservations;
    std::map<ObservationBand, Rinex::Observation::SignalStrength> SnrObservations;
};
