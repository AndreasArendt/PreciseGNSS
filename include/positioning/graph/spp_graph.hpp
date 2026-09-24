#pragma once

#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"
#include <gtsam/nonlinear/Values.h>

class SppGraph {
//  using TemporalModel =
//      std::variant<StaticModel, ConstantVelocityModel, ImuModel>;

public:
  // One initial state per epoch; corrections and noise are evaluated there.
  gtsam::Values Solve(const std::vector<PositioningEpoch> &epochs,
                      const std::vector<ReceiverState> &initialStates) const;
};
