#include "coordinates/wgs84.hpp"

#include <iostream>

int main() {
    const constants::Lla station{
        52.520008,
        13.404954,
        45.0,
    };

    const constants::Ecef ecef = constants::lla_to_ecef(station);
    const gtsam::Point3 point = constants::to_gtsam_point3(ecef);

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
