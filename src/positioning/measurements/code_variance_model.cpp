#include "positioning/measurements/code_variance_model.hpp"
#include "coordinates/transformation.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
void checkConfig(const CodeVarianceConfig &config) {
  const auto finiteNonnegative = [](double value) {
    return std::isfinite(value) && value >= 0.0;
  };

  if (!std::isfinite(config.fallback_sigma_m) ||
      config.fallback_sigma_m <= 0.0 || !std::isfinite(config.floor_sigma_m) ||
      config.floor_sigma_m <= 0.0 ||
      !finiteNonnegative(config.elevation_sigma_m) ||
      !finiteNonnegative(config.cn0_sigma_at_ref_m) ||
      !std::isfinite(config.reference_cn0_dbhz) ||
      !std::isfinite(config.missing_cn0_dbhz) ||
      !std::isfinite(config.min_elevation__rad) ||
      config.min_elevation__rad <= 0.0 ||
      config.min_elevation__rad > std::numbers::pi / 2.0) {
    throw std::invalid_argument("Invalid code variance configuration");
  }
}
} // namespace

double CodeVarianceModel(const CodeObservation &observation,
                         const CodeVarianceContext &context,
                         const CodeVarianceConfig &config) {

  // verify that config is correct to avoid problems in estimator
  checkConfig(config);

  double variance_m2 = config.fallback_sigma_m * config.fallback_sigma_m;

  if (!context.elevation_rad || !std::isfinite(*context.elevation_rad)) {
    return variance_m2;
  }

  // elevation dependency
  const double effectiveElevation =
      std::max(*context.elevation_rad, config.min_elevation__rad);

  variance_m2 = std::pow(config.floor_sigma_m, 2) +
                std::pow(config.elevation_sigma_m, 2) /
                    std::pow(std::sin(effectiveElevation), 2);

  // cno dependent term (optional if configured)
  if (config.cn0_sigma_at_ref_m > 0.0) {
    const double cn0 = context.cn0_dbhz && std::isfinite(*context.cn0_dbhz)
                           ? *context.cn0_dbhz
                           : config.missing_cn0_dbhz;
    variance_m2 += std::pow(config.cn0_sigma_at_ref_m, 2) *
                   std::pow(10.0, (config.reference_cn0_dbhz - cn0) / 10.0);
  }

  return variance_m2;
}
