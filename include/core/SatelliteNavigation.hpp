#pragma once

#include "core/satellite_id.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"

#include <vector>
#include <variant>

using NavigationMessage = std::variant<
    GpsNavData,
    GalileoNavData
>;

struct SatelliteNavigation
{
    SatelliteId satellite;
    std::vector<NavigationMessage> messages;    
};
