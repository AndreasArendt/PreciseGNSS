#pragma once

#include "core/satellite_id.hpp"
#include "rinex/Observations.hpp"
#include "core/SatelliteObservation.hpp"

// One code measurement for a satellite and signal at the evaluated epoch.
struct CodeObservation
{
    SatelliteId satellite;
    SignalId signalId;
    Rinex::Observation::Code code{};
};
