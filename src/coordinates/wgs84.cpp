#include "gnss_rtk/coordinates/wgs84.hpp"

#include <cmath>
#include <numbers>

namespace {

double deg_to_rad(const double degrees) {
    return degrees * std::numbers::phi / 180.0;
}

double rad_to_deg(const double radians) {
    return radians * 180.0 / std::numbers::phi;
}

}  // namespace

namespace gnss_rtk {

Ecef lla_to_ecef(const Lla& lla) {
    const double latitude_rad = deg_to_rad(lla.latitude_deg);
    const double longitude_rad = deg_to_rad(lla.longitude_deg);

    const double first_eccentricity_squared =
        kWgs84Flattening * (2.0 - kWgs84Flattening);

    const double sin_latitude = std::sin(latitude_rad);
    const double cos_latitude = std::cos(latitude_rad);
    const double sin_longitude = std::sin(longitude_rad);
    const double cos_longitude = std::cos(longitude_rad);

    const double prime_vertical_radius =
        kWgs84SemiMajorAxisM /
        std::sqrt(1.0 - first_eccentricity_squared * sin_latitude * sin_latitude);

    return {
        (prime_vertical_radius + lla.altitude_m) * cos_latitude * cos_longitude,
        (prime_vertical_radius + lla.altitude_m) * cos_latitude * sin_longitude,
        (prime_vertical_radius * (1.0 - first_eccentricity_squared) + lla.altitude_m) *
            sin_latitude,
    };
}

Lla ecef_to_lla(const Ecef& ecef) {
    const double first_eccentricity_squared =
        kWgs84Flattening * (2.0 - kWgs84Flattening);
    const double second_eccentricity_squared =
        (kWgs84SemiMajorAxisM * kWgs84SemiMajorAxisM -
         kWgs84SemiMinorAxisM * kWgs84SemiMinorAxisM) /
        (kWgs84SemiMinorAxisM * kWgs84SemiMinorAxisM);

    const double p = std::hypot(ecef.x_m, ecef.y_m);
    const double theta =
        std::atan2(ecef.z_m * kWgs84SemiMajorAxisM, p * kWgs84SemiMinorAxisM);

    const double sin_theta = std::sin(theta);
    const double cos_theta = std::cos(theta);

    const double latitude_rad = std::atan2(
        ecef.z_m +
            second_eccentricity_squared * kWgs84SemiMinorAxisM * sin_theta * sin_theta *
                sin_theta,
        p - first_eccentricity_squared * kWgs84SemiMajorAxisM * cos_theta * cos_theta *
                cos_theta);

    const double longitude_rad = std::atan2(ecef.y_m, ecef.x_m);
    const double sin_latitude = std::sin(latitude_rad);

    const double prime_vertical_radius =
        kWgs84SemiMajorAxisM /
        std::sqrt(1.0 - first_eccentricity_squared * sin_latitude * sin_latitude);

    double altitude_m = 0.0;
    if (std::abs(std::cos(latitude_rad)) > 1.0e-12) {
        altitude_m = p / std::cos(latitude_rad) - prime_vertical_radius;
    } else {
        altitude_m =
            ecef.z_m / sin_latitude -
            prime_vertical_radius * (1.0 - first_eccentricity_squared);
    }

    return {
        rad_to_deg(latitude_rad),
        rad_to_deg(longitude_rad),
        altitude_m,
    };
}

double distance_m(const Ecef& lhs, const Ecef& rhs) {
    const double dx = lhs.x_m - rhs.x_m;
    const double dy = lhs.y_m - rhs.y_m;
    const double dz = lhs.z_m - rhs.z_m;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

gtsam::Point3 to_gtsam_point3(const Ecef& ecef) {
    return {ecef.x_m, ecef.y_m, ecef.z_m};
}

}  // namespace gnss_rtk
