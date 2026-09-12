#include <Eigen/Dense>
#include <cmath>
#include <optional>

#include "estimation/robust_weighting.hpp"
#include "positioning/estimators/spp_solver.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

namespace {

struct LinearizedSystem {
  Eigen::MatrixXd jacobian;
  Eigen::VectorXd residuals;
  Eigen::VectorXd variances_m2;
};

double ObservationVarianceModel(){
  
}

bool LinearizeEpoch(const PositioningEpoch &epoch,
                    const ReceiverState &receiver,
                    const CorrectionContext &context,
                    LinearizedSystem &linearizedSystem) {
  Eigen::Index row = 0;

  for (const auto &measurement : epoch.measurements) {
    for (const auto &codeObs : measurement.observations.CodeObservations) {
      const CodeObservation observation{
          .satellite = measurement.observations.satellite,
          .band = codeObs.first.band,
          .attribute = codeObs.first.attribute,
          .pseudorange_m = codeObs.second.pseudorange_m};
      PseudorangeModel model;
      const auto prediction = model.Evaluate(
          observation, receiver, measurement.satelliteState, context);

      linearizedSystem.residuals(row) =
          observation.pseudorange_m - prediction.predicted_m;

      linearizedSystem.variances_m2(row) = 5.0 * 5.0;

      for (Eigen::Index column = 0; column < 3; ++column) {
        linearizedSystem.jacobian(row, column) =
            prediction.d_predicted_d_receiver_position[column];
      }
      linearizedSystem.jacobian(row, 3) =
          prediction.d_predicted_d_receiver_clock_bias;
      ++row;
    }
  }
  return linearizedSystem.jacobian.allFinite() &&
         linearizedSystem.residuals.allFinite();
}

std::optional<ReceiverState> SolveEpoch(const PositioningEpoch &epoch) {
  Eigen::Index rowCount = 0;
  for (const auto &measurement : epoch.measurements) {
    rowCount += static_cast<Eigen::Index>(
        measurement.observations.CodeObservations.size());
  }
  if (rowCount < 4)
    return std::nullopt;

  ReceiverState receiver{};
  const CorrectionContext context{};
  LinearizedSystem linearizedSystem{
      .jacobian = Eigen::MatrixXd(rowCount, 4),
      .residuals = Eigen::VectorXd(rowCount),
      .variances_m2 = Eigen::VectorXd(rowCount),
  };

  constexpr int maxIterations = 20;
  constexpr double tolerance_m = 1e-3;
  for (int iteration = 0; iteration < maxIterations; ++iteration) {
    if (!LinearizeEpoch(epoch, receiver, context, linearizedSystem))
      return std::nullopt;

    Eigen::MatrixXd weightedH = linearizedSystem.jacobian;
    Eigen::VectorXd weightedResiduals = linearizedSystem.residuals;
    ApplyHuberWeights(weightedH, weightedResiduals, linearizedSystem.variances_m2);

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

    if (dx.head<3>().norm() < tolerance_m && std::abs(dx(3)) < tolerance_m)
      return receiver;
  }
  return std::nullopt;
}

} // namespace

std::vector<SPPState>
SPPSolver::Solve(const std::vector<PositioningEpoch> &epochs) {
  std::vector<SPPState> states;
  states.reserve(epochs.size());
  for (const auto &epoch : epochs) {
    states.push_back(
        {.receiverState = SolveEpoch(epoch), .epochtime = epoch.receptionTime});
  }
  return states;
}
