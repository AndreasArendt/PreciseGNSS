#pragma once

#include <optional>
#include <vector>

#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"

struct SPPState {
  std::optional<ReceiverState> receiverState;
  navigation::GnssTime epochtime;
};

struct SolverConfig {
  unsigned int maxIterations = 100;
  double position_tolerance_m = 1e-3;
  double clock_tolerance_m = 1e-3;
};

class SPPSolver {

private:
public:
  std::vector<SPPState> Solve(const std::vector<PositioningEpoch> &epochs,
                              const SolverConfig &solverConfig = {});
};