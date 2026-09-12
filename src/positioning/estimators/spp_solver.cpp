#include <Eigen/Dense>
#include <cmath>
#include <optional>

#include "estimation/robust_weighting.hpp"
#include "positioning/estimators/spp_solver.hpp"
#include "positioning/measurements/linearization/pseudorange_linearizer.hpp"

namespace {

EpochSolveResult SolveEpoch(const PositioningEpoch &epoch,
                            const std::optional<ReceiverState> &initial,
                            const SolverConfig &solverConfig) {

  EpochSolveResult result{
      .receiverState = std::nullopt,
      .status = SolveStatus::MaxIterations,
      .diagnostics = {},
      .receptionTime = epoch.receptionTime,
  };

  // get initial guess
  ReceiverState receiver{};
  if (initial) {
    receiver = *initial;
  }

  const CorrectionContext context{};

  for (int iteration = 0; iteration < solverConfig.maxIterations; ++iteration) {
    result.diagnostics.iterations = iteration + 1;

    const auto linearizedSystem =
        LinearizePseudorangeEpoch(epoch, receiver, context);

    if (!linearizedSystem) {
      result.status = SolveStatus::InvalidLinearization;
      return result;
    }
    if (linearizedSystem->jacobian.rows() < linearizedSystem->jacobian.cols()) {
      result.status = SolveStatus::InsufficientMeasurements;
      return result;
    }

    Eigen::MatrixXd weightedH = linearizedSystem->jacobian;
    Eigen::VectorXd weightedResiduals = linearizedSystem->residuals;
    ApplyHuberWeights(weightedH, weightedResiduals,
                      linearizedSystem->variances_m2);

    auto qr = weightedH.colPivHouseholderQr();
    if (qr.rank() < weightedH.cols()) {
      result.status = SolveStatus::RankDeficient;
      return result;
    }
    Eigen::Vector4d dx = qr.solve(weightedResiduals);
    if (!dx.allFinite()) {
      result.status = SolveStatus::NonFiniteUpdate;
      return result;
    }

    receiver.position = ECEF_Position{receiver.position.x() + dx(0),
                                      receiver.position.y() + dx(1),
                                      receiver.position.z() + dx(2)};
    receiver.clockBias_m += dx(3);

    if (dx.head<3>().norm() < solverConfig.position_tolerance_m &&
        std::abs(dx(3)) < solverConfig.clock_tolerance_m) {
      const Eigen::Matrix4d inverseR = qr.matrixR()
                                           .topLeftCorner<4, 4>()
                                           .triangularView<Eigen::Upper>()
                                           .solve(Eigen::Matrix4d::Identity());

      const Eigen::Vector4d variances =
          qr.colsPermutation() * inverseR.rowwise().squaredNorm();

      receiver.pos_variance__m2 = variances.head<3>();
      receiver.clockBias_variance__m2 = variances(3);
      result.receiverState = receiver;
      result.status = SolveStatus::Converged;
      return result;
    }
  }

  return result;
}

} // namespace

std::vector<EpochSolveResult>
SPPSolver::Solve(const std::vector<PositioningEpoch> &epochs,
                 const SolverConfig &solverConfig) {

  std::vector<EpochSolveResult> results;
  results.reserve(epochs.size());

  std::optional<ReceiverState> receiverState{};
  for (const auto &epoch : epochs) {

    // passing initial/previous receiverState
    EpochSolveResult result = SolveEpoch(epoch, receiverState, solverConfig);
    receiverState = result.receiverState;

    results.push_back(result);
  }

  return results;
}
