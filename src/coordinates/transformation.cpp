#include <algorithm>
#include <cmath>

#include "coordinates/transformation.hpp"
#include "core/constants.hpp"

double elevationFromEcef(const Eigen::Vector3d &receiver_ecef,
                         const Eigen::Vector3d &satellite_ecef) {
  constexpr double a = constants::WGS84SemiMajorAxis__m;
  constexpr double e2 = constants::kWgs84FirstEccentricitySquared;

  const double x = receiver_ecef.x();
  const double y = receiver_ecef.y();
  const double z = receiver_ecef.z();

  const double p = std::hypot(x, y);

  const double lon = std::atan2(y, x);

  double lat = std::atan2(z, p * (1.0 - e2));

  for (int i = 0; i < 10; ++i) {
    const double sin_lat = std::sin(lat);

    const double N = a / std::sqrt(1.0 - e2 * sin_lat * sin_lat);

    const double h = p / std::cos(lat) - N;

    const double new_lat = std::atan2(z, p * (1.0 - e2 * N / (N + h)));

    if (std::abs(new_lat - lat) < 1e-13) {
      lat = new_lat;
      break;
    }

    lat = new_lat;
  }

  const Eigen::Vector3d up{std::cos(lat) * std::cos(lon),
                           std::cos(lat) * std::sin(lon), std::sin(lat)};

  const Eigen::Vector3d los = satellite_ecef - receiver_ecef;

  const double sin_elevation = los.dot(up) / los.norm();

  return std::asin(std::clamp(sin_elevation, -1.0, 1.0));
}