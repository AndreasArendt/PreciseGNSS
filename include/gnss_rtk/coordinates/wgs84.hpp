#pragma once

#include <gtsam/geometry/Point3.h>

namespace gnss_rtk {

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

constexpr double kWgs84SemiMajorAxisM = 6378137.0;
constexpr double kWgs84Flattening = 1.0 / 298.257223563;
constexpr double kWgs84SemiMinorAxisM =
    kWgs84SemiMajorAxisM * (1.0 - kWgs84Flattening);

Ecef lla_to_ecef(const Lla& lla);
Lla ecef_to_lla(const Ecef& ecef);
double distance_m(const Ecef& lhs, const Ecef& rhs);
gtsam::Point3 to_gtsam_point3(const Ecef& ecef);

}  // namespace gnss_rtk
