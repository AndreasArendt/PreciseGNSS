#pragma once

#include <numbers>
#include <optional>

#include "positioning/measurements/code_observation.hpp"

struct CodeVarianceConfig {
  double fallback_sigma_m = 5.0;

  double floor_sigma_m = 1.0;
  double elevation_sigma_m = 0.5;

  double cn0_sigma_at_ref_m = 0.5; //setting this to 0 disables cno term
  double reference_cn0_dbhz = 45.0;
  double missing_cn0_dbhz = 35.0;

  double min_elevation__rad = 5.0 * (std::numbers::pi / 180.0);
};

struct CodeVarianceContext {
  std::optional<double> elevation_rad;
  // Supply only when the signal-strength units are known to be dB-Hz.
  std::optional<double> cn0_dbhz;
};

// Returns code measurement variance in m^2, excluding correction uncertainty.
double CodeVarianceModel(const CodeObservation &observation,
                         const CodeVarianceContext &context = {},
                         const CodeVarianceConfig &config = {});
