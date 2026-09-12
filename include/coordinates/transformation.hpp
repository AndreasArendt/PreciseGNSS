#pragma once

#include <Eigen/Core>

// Returns elevation in radians.
double elevationFromEcef(const Eigen::Vector3d &receiver_ecef,
                         const Eigen::Vector3d &satellite_ecef);
