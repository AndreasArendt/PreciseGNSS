#include "positioning/measurements/code_variance_model.hpp"
#include "coordinates/transformation.hpp"

#include <cmath>
#include <stdexcept>

double CodeVarianceModel(const CodeObservation &observation,
                         const CodeVarianceContext &context,
                         const CodeVarianceConfig &config) {
  double variance_m2 = config.fallback_sigma_m * config.fallback_sigma_m;

  if (config.fallback_sigma_m == 0.0)
    throw std::invalid_argument("Sigma must never be 0");

  if (auto elevation_rad = context.elevation_rad) {
    double a = 0.4; // Zenith-constant term
    double b = 0.4; // elevation term

    variance_m2 =
        std::pow(a, 2) + std::pow(b, 2) / std::pow(std::sin(*elevation_rad), 2);
  }

  return variance_m2;
}
