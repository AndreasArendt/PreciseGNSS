#pragma once

#include "gnss_rtk/coordinates/ecef_position.hpp"
#include "gnss_rtk/coordinates/position.hpp"
#include "gnss_rtk/core/ObservationEpoch.hpp"
#include "gnss_rtk/core/SatelliteNavigation.hpp"
#include "gnss_rtk/rinex/RinexTypes/IonosphericCorrection.hpp"
#include "gnss_rtk/rinex/RinexTypes/TimeSystemCorrection.hpp"

#include <string>
#include <variant>
#include <vector>

struct ObservationFile {
    std::string version;
    ECEF_Position approximateMarkerPosition;
    Position antennaOffset;
    std::vector<ObservationEpoch> epochs;
};

struct NavigationFile {
    std::string version;
    std::string type;
    std::vector<IonosphericCorrection> ionosphericCorrections;
    std::vector<TimeSystemCorrection> timeSystemCorrections;
    std::vector<SatelliteNavigation> satellites;
};

using RinexFile = std::variant<ObservationFile, NavigationFile>;
