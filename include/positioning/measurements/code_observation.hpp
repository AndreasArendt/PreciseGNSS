#pragma once

#include "core/satellite_id.hpp"
#include "rinex/RinexTypes/ObservationAttribute.hpp"
#include "rinex/RinexTypes/ObservationBand.hpp"

// One code measurement for a satellite and signal at the evaluated epoch.
struct CodeObservation
{
    SatelliteId satellite;
    ObservationBand band{ObservationBand::Band_Unkown};
    ObservationAttribute attribute{ObservationAttribute::Channel_Unknown};
    double pseudorange_m{};
};
