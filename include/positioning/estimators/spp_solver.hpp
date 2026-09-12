#pragma once

#include <optional>
#include <vector>

#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"

struct SolverConfig {
  int maxIterations = 100;
  double position_tolerance_m = 1e-3;
  double clock_tolerance_m = 1e-3;
};

enum class SolveStatus {
  Converged,
  InsufficientMeasurements,
  RankDeficient,
  InvalidLinearization,
  NonFiniteUpdate,
  MaxIterations
};

struct SolverDiagnostics {
  int iterations = 0;
  std::optional<double> residualRms_m;
};

struct EpochSolveResult {
  std::optional<ReceiverState> receiverState{};
  SolveStatus status{};
  SolverDiagnostics diagnostics{};
  navigation::GnssTime receptionTime{};
};

class SPPSolver {

private:
public:
  std::vector<EpochSolveResult>
  Solve(const std::vector<PositioningEpoch> &epochs,
        const SolverConfig &solverConfig = {});
};