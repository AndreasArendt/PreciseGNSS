#pragma once

#include "gnss_rtk/core/satellite_id.hpp"
#include "gnss_rtk/rinex/NavData/Galileo/GalileoNavData.hpp"
#include "gnss_rtk/rinex/NavData/Gps/GpsNavData.hpp"

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
