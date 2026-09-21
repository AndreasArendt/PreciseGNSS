#pragma once

#include <optional>

namespace troposphere {

struct Input {
    double latitude_rad;   // Geodetic latitude
    double height_m;       // Above mean sea level
    double elevation_rad; // Above the local horizon
};

struct Delay {
    double hydrostatic_m;  // Already mapped to satellite elevation
    double wet_m;

    double Total() const {
        return hydrostatic_m + wet_m;
    }
};

// A model is any function with this signature.
using Model = std::optional<Delay> (*)(const Input&);

} // namespace troposphere
