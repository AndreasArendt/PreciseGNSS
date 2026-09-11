#include <Eigen/Dense>
#include <cmath>
#include <optional>

#include "positioning/estimators/spp_solver.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

namespace {

bool LinearizeEpoch(const PositioningEpoch &epoch,
                    const ReceiverState &receiver,
                    const CorrectionContext &context, Eigen::MatrixXd &H,
                    Eigen::VectorXd &residuals) {
  Eigen::Index row = 0;
  for (const auto &measurement : epoch.measurements) {
    for (const auto &codeObs : measurement.observations.CodeObservations) {
      const CodeObservation observation{
          .satellite = measurement.observations.satellite,
          .band = codeObs.first,
          .attribute = ObservationAttribute::Channel_Unknown,
          .pseudorange_m = codeObs.second.pseudorange_m,
          .variance_m2 = 0.0};
      PseudorangeModel model;
      const auto prediction = model.Evaluate(
          observation, receiver, measurement.satelliteState, context);

      residuals(row) = observation.pseudorange_m - prediction.predicted_m;
      for (Eigen::Index column = 0; column < 3; ++column) {
        H(row, column) = prediction.d_predicted_d_receiver_position[column];
      }
      H(row, 3) = prediction.d_predicted_d_receiver_clock_bias;
      ++row;
    }
  }
  return H.allFinite() && residuals.allFinite();
}

void ApplyHuberWeights(Eigen::MatrixXd &H, Eigen::VectorXd &residuals) {
  constexpr double huber_k = 1.345; // Huber default
  constexpr double sigma_m = 5.0;   // TODO: Model measurement uncertainty.

  for (Eigen::Index row = 0; row < residuals.size(); ++row) {
    const double absU = std::abs(residuals(row)) / sigma_m;
    const double weight = absU <= huber_k ? 1.0 : huber_k / absU;
    const double scale = std::sqrt(weight) / sigma_m;
    H.row(row) *= scale;
    residuals(row) *= scale;
  }
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
  Eigen::MatrixXd H(rowCount, 4);
  Eigen::VectorXd residuals(rowCount);

  constexpr int maxIterations = 20;
  constexpr double tolerance_m = 1e-3;
  for (int iteration = 0; iteration < maxIterations; ++iteration) {
    if (!LinearizeEpoch(epoch, receiver, context, H, residuals))
      return std::nullopt;

    Eigen::MatrixXd weightedH = H;
    Eigen::VectorXd weightedResiduals = residuals;
    ApplyHuberWeights(weightedH, weightedResiduals);

    auto qr = weightedH.colPivHouseholderQr();
    if (qr.rank() < H.cols())
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
