#include "estimation/robust_weighting.hpp"

#include <cmath>

void ApplyHuberWeights(Eigen::MatrixXd &H, Eigen::VectorXd &residuals,
                       const Eigen::VectorXd &variances,
                       const HuberConfig &config) {
  for (Eigen::Index row = 0; row < residuals.size(); ++row) {
    const double sigma = std::sqrt(variances(row));
    const double absU = std::abs(residuals(row)) / sigma;
    const double weight = absU <= config.huber_k ? 1.0 : config.huber_k / absU;
    const double scale = std::sqrt(weight) / sigma;
    H.row(row) *= scale;
    residuals(row) *= scale;
  }
}
