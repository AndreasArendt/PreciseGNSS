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

  // get initial guess
  ReceiverState receiver{};
  if (initial) {
    receiver = *initial;
  }

  const CorrectionContext context{};

  for (int iteration = 1; iteration <= solverConfig.maxIterations;
       ++iteration) {
    const auto linearizedSystem =
        LinearizePseudorangeEpoch(epoch, receiver, context);

    if (!linearizedSystem) {
      return EpochSolveResult{
          .receiverState = std::nullopt,
          .status = SolveStatus::InvalidLinearization,
          .diagnostics = SolverDiagnostics{.iterations = iteration,
                                           .residualRms_m = std::nullopt}};
    }
    if (linearizedSystem->jacobian.rows() < linearizedSystem->jacobian.cols()) {
      return EpochSolveResult{
          .receiverState = std::nullopt,
          .status = SolveStatus::InsufficientMeasurements,
          .diagnostics = SolverDiagnostics{.iterations = iteration,
                                           .residualRms_m = std::nullopt}};
    }

    Eigen::MatrixXd weightedH = linearizedSystem->jacobian;
    Eigen::VectorXd weightedResiduals = linearizedSystem->residuals;
    ApplyHuberWeights(weightedH, weightedResiduals,
                      linearizedSystem->variances_m2);

    auto qr = weightedH.colPivHouseholderQr();
    if (qr.rank() < weightedH.cols()) {
      return EpochSolveResult{
          .receiverState = std::nullopt,
          .status = SolveStatus::RankDeficient,
          .diagnostics = SolverDiagnostics{.iterations = iteration,
                                           .residualRms_m = std::nullopt}};
    }
    Eigen::Vector4d dx = qr.solve(weightedResiduals);
    if (!dx.allFinite()) {
      return EpochSolveResult{
          .receiverState = std::nullopt,
          .status = SolveStatus::NonFiniteUpdate,
          .diagnostics = SolverDiagnostics{.iterations = iteration,
                                           .residualRms_m = std::nullopt}};
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

      return EpochSolveResult{
          .receiverState =
              ReceiverState{.position = receiver.position,
                            .clockBias_m = receiver.clockBias_m,
                            .pos_variance__m2 = variances.head<3>(),
                            .clockBias_variance__m2 = variances(3)},
          .status = SolveStatus::Converged,
          .diagnostics = SolverDiagnostics{.iterations = iteration,
                                           .residualRms_m = std::nullopt}};
    }
  }

  return EpochSolveResult{
      .receiverState = std::nullopt,
      .status = SolveStatus::MaxIterations,
      .diagnostics = SolverDiagnostics{.iterations = solverConfig.maxIterations,
                                       .residualRms_m = std::nullopt}};
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
    result.receptionTime = epoch.receptionTime;
    receiverState = result.receiverState;

    results.push_back(result);
  }

  return results;
}
