#pragma once

#include "coordinates/position.hpp"
#include "coordinates/wgs84.hpp"

class ECEF_Position : public Position {
private:
public:
  Lla toWgs84() const {
    return constants::ecef_to_lla(
        {.x_m = this->x(), .y_m = this->y(), .z_m = this->z()});
  }

  // ctor & dtor
  ECEF_Position() : Position() {}
  ECEF_Position(double x, double y, double z) : Position(x, y, z) {}
  explicit ECEF_Position(const Eigen::Vector3d &coordinates_m)
      : Position(coordinates_m) {}
  ~ECEF_Position() {}
};
