#pragma once

#include <Eigen/Core>

struct HuberConfig {
  double huber_k = 1.345;
};

// Scales Jacobian rows and residuals in place.
void ApplyHuberWeights(Eigen::MatrixXd &H, Eigen::VectorXd &residuals,
                       const Eigen::VectorXd &variances,
                       const HuberConfig &config = {});
