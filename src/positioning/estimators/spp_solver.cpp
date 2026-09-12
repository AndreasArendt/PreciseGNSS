#include <Eigen/Dense>
#include <cmath>
#include <optional>

#include "estimation/robust_weighting.hpp"
#include "positioning/estimators/spp_solver.hpp"
#include "positioning/measurements/linearization/pseudorange_linearizer.hpp"

namespace {

std::optional<ReceiverState>
SolveEpoch(const PositioningEpoch &epoch,
           const std::optional<ReceiverState> &initial) {

  Eigen::Index rowCount = 0;
  for (const auto &measurement : epoch.measurements) {
    for (const auto &[signalId, observation] :
         measurement.observations.signals) {
      if (observation.CodeObservation)
        ++rowCount;
    }
  }

  if (rowCount < 4)
    return std::nullopt;

  // get initial guess
  ReceiverState receiver{};
  if (initial) {
    receiver = *initial;
  }

  const CorrectionContext context{};
  LinearizedSystem linearizedSystem{
      .jacobian = Eigen::MatrixXd(rowCount, 4),
      .residuals = Eigen::VectorXd(rowCount),
      .variances_m2 = Eigen::VectorXd(rowCount),
  };

  constexpr int maxIterations = 100;
  constexpr double tolerance_m = 1e-3;
  for (int iteration = 0; iteration < maxIterations; ++iteration) {
    if (!LinearizeEpoch(epoch, receiver, context, linearizedSystem))
      return std::nullopt;

    Eigen::MatrixXd weightedH = linearizedSystem.jacobian;
    Eigen::VectorXd weightedResiduals = linearizedSystem.residuals;
    ApplyHuberWeights(weightedH, weightedResiduals,
                      linearizedSystem.variances_m2);

    auto qr = weightedH.colPivHouseholderQr();
    if (qr.rank() < weightedH.cols())
      return std::nullopt;

    Eigen::Vector4d dx = qr.solve(weightedResiduals);
    if (!dx.allFinite())
      return std::nullopt;

    receiver.position = ECEF_Position{receiver.position.x() + dx(0),
                                      receiver.position.y() + dx(1),
                                      receiver.position.z() + dx(2)};
    receiver.clockBias_m += dx(3);

    if (dx.head<3>().norm() < tolerance_m && std::abs(dx(3)) < tolerance_m) {
      const Eigen::Matrix4d inverseR = qr.matrixR()
                                           .topLeftCorner<4, 4>()
                                           .triangularView<Eigen::Upper>()
                                           .solve(Eigen::Matrix4d::Identity());

      const Eigen::Vector4d variances =
          qr.colsPermutation() * inverseR.rowwise().squaredNorm();

      return ReceiverState{.position = receiver.position,
                           .clockBias_m = receiver.clockBias_m,
                           .pos_variance__m2 = variances.head<3>(),
                           .clockBias_variance__m2 = variances(3)};
    }
  }
  return std::nullopt;
}

} // namespace

std::vector<SPPState>
SPPSolver::Solve(const std::vector<PositioningEpoch> &epochs) {
  std::vector<SPPState> states;
  states.reserve(epochs.size());

  std::optional<ReceiverState> receiverState{};
  for (const auto &epoch : epochs) {

    // passing initial/previous receiverState
    receiverState = SolveEpoch(epoch, receiverState);

    states.push_back(
        {.receiverState = receiverState, .epochtime = epoch.receptionTime});
  }
  return states;
}
