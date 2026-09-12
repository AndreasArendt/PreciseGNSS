#pragma once

#include <Eigen/Core>

class Position {
private:
  Eigen::Vector3d coordinates_m_ = Eigen::Vector3d::Zero();

public:
  // getters
  double x() const { return coordinates_m_.x(); }
  double y() const { return coordinates_m_.y(); }
  double z() const { return coordinates_m_.z(); }
  const Eigen::Vector3d &vector() const { return coordinates_m_; }

  double norm() const { return coordinates_m_.stableNorm(); }

  // ctor & dtor
  Position() = default;
  Position(double x, double y, double z) : coordinates_m_(x, y, z) {}
  explicit Position(const Eigen::Vector3d &coordinates_m)
      : coordinates_m_(coordinates_m) {}
  ~Position() = default;

  Position operator+(const Position &rhs) const {
    return Position(coordinates_m_ + rhs.coordinates_m_);
  }

  Position operator-(const Position &rhs) const {
    return Position(coordinates_m_ - rhs.coordinates_m_);
  }
};
