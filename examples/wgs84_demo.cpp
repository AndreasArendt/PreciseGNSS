#include "gnss_rtk/wgs84.hpp"

#include <iostream>

int main() {
    const gnss_rtk::Lla station{
        52.520008,
        13.404954,
        45.0,
    };

    const gnss_rtk::Ecef ecef = gnss_rtk::lla_to_ecef(station);
    const gtsam::Point3 point = gnss_rtk::to_gtsam_point3(ecef);

    std::cout << "ECEF [m]: "
              << ecef.x_m << ", "
              << ecef.y_m << ", "
              << ecef.z_m << '\n';

    std::cout << "GTSAM Point3 [m]: "
              << point.x() << ", "
              << point.y() << ", "
              << point.z() << '\n';

    return 0;
}
