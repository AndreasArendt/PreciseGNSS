#pragma once

#include <optional>

#include "positioning/measurements/code_observation.hpp"

struct CodeVarianceConfig {
  double fallback_sigma_m = 5.0;
};

struct CodeVarianceContext {
  std::optional<double> elevation_rad;
  // Supply only when the signal-strength units are known to be dB-Hz.
  std::optional<double> cn0_dbhz;
};

// Returns code measurement variance in m², excluding correction uncertainty.
// Currently uses only the configured fallback; quality modeling is pending.
double CodeVarianceModel(const CodeObservation &observation,
                         const CodeVarianceContext &context = {},
                         const CodeVarianceConfig &config = {});
