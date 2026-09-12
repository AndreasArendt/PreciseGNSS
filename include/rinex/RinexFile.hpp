#pragma once

#include "coordinates/ecef_position.hpp"
#include "coordinates/position.hpp"
#include "core/ObservationEpoch.hpp"
#include "core/SatelliteNavigation.hpp"
#include "rinex/RinexTypes/IonosphericCorrection.hpp"
#include "rinex/RinexTypes/TimeSystemCorrection.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>

struct ObservationFile {
    std::string version;
    ECEF_Position approximateMarkerPosition;
    Position antennaOffset;
    std::vector<ObservationEpoch> epochs;
    // Header value (typically DBHZ); absent means unspecified.
    std::optional<std::string> signalStrengthUnit;
};

struct NavigationFile {
    std::string version;
    std::string type;
    std::vector<IonosphericCorrection> ionosphericCorrections;
    std::vector<TimeSystemCorrection> timeSystemCorrections;
    std::vector<SatelliteNavigation> satellites;
};

using RinexFile = std::variant<ObservationFile, NavigationFile>;
