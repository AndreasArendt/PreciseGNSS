#include "positioning/measurements/code_variance_model.hpp"

#include <cmath>
#include <stdexcept>

double CodeVarianceModel(const CodeObservation &, const CodeVarianceContext &,
                         const CodeVarianceConfig &config) {
  const double variance_m2 = config.fallback_sigma_m * config.fallback_sigma_m;
  if (config.fallback_sigma_m <= 0.0 || !std::isfinite(variance_m2) ||
      variance_m2 <= 0.0)
    throw std::invalid_argument(
        "Code variance requires a finite positive sigma and variance");

  // TODO: Model signal-dependent noise, elevation, and C/N0 with SSI fallback.
  return variance_m2;
}
