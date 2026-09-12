#pragma once

#include <Eigen/Core>

struct LinearizedSystem {
  Eigen::MatrixXd jacobian;
  Eigen::VectorXd residuals;
  Eigen::VectorXd variances_m2;
};