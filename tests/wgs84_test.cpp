#include "coordinates/wgs84.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

bool nearly_equal(const double lhs, const double rhs, const double tolerance) {
    return std::abs(lhs - rhs) <= tolerance;
}

int fail(const char* label, const double expected, const double actual) {
    std::cerr << label << " expected " << expected << " but got " << actual << '\n';
    return EXIT_FAILURE;
}

}  // namespace

int main() {
    const gnss_rtk::Lla berlin{
        52.520008,
        13.404954,
        45.0,
    };

    const gnss_rtk::Ecef ecef = gnss_rtk::lla_to_ecef(berlin);
    const gnss_rtk::Lla round_trip = gnss_rtk::ecef_to_lla(ecef);
    const gtsam::Point3 point = gnss_rtk::to_gtsam_point3(ecef);

    if (!nearly_equal(berlin.latitude_deg, round_trip.latitude_deg, 1.0e-6)) {
        return fail("latitude", berlin.latitude_deg, round_trip.latitude_deg);
    }

    if (!nearly_equal(berlin.longitude_deg, round_trip.longitude_deg, 1.0e-6)) {
        return fail("longitude", berlin.longitude_deg, round_trip.longitude_deg);
    }

    if (!nearly_equal(berlin.altitude_m, round_trip.altitude_m, 1.0e-3)) {
        return fail("altitude", berlin.altitude_m, round_trip.altitude_m);
    }

    if (!nearly_equal(point.x(), ecef.x_m, 1.0e-9)) {
        return fail("point.x", ecef.x_m, point.x());
    }

    if (!nearly_equal(point.y(), ecef.y_m, 1.0e-9)) {
        return fail("point.y", ecef.y_m, point.y());
    }

    if (!nearly_equal(point.z(), ecef.z_m, 1.0e-9)) {
        return fail("point.z", ecef.z_m, point.z());
    }

    return EXIT_SUCCESS;
}
