#pragma once

#include "positioning/correction_context.hpp"
#include "positioning/measurements/code_observation.hpp"
#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"
#include <vector>

struct PreparedCodeMeasurement {
  CodeObservation observation;
  SatelliteState satellite;
  CorrectionContext corrections;
  double variance_m2;
};

// Values are evaluated at referenceReceiver. No prediction, Jacobian, or
// solver-specific weighting is applied. Preserves epoch/signal iteration order.
// Retains the existing policy: unavailable troposphere contributes zero.
std::vector<PreparedCodeMeasurement>
PrepareCodeEpoch(const PositioningEpoch &epoch,
                 const ReceiverState &referenceReceiver);
