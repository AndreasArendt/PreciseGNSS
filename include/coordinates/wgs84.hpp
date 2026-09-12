#pragma once

#include <gtsam/geometry/Point3.h>

#include "core/constants.hpp"

namespace constants {

struct Lla {
    double latitude_deg{};
    double longitude_deg{};
    double altitude_m{};
};

struct Ecef {
    double x_m{};
    double y_m{};
    double z_m{};
};

Ecef lla_to_ecef(const Lla& lla);
Lla ecef_to_lla(const Ecef& ecef);
double distance_m(const Ecef& lhs, const Ecef& rhs);
gtsam::Point3 to_gtsam_point3(const Ecef& ecef);

}  // namespace constants
