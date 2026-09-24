#include "positioning/measurements/linearization/pseudorange_linearizer.hpp"
#include "positioning/measurements/prepare_code_epoch.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

std::optional<LinearizedSystem> LinearizePseudorangeEpoch(
    const PositioningEpoch &epoch, const ReceiverState &receiver) {
  const auto prepared = PrepareCodeEpoch(epoch, receiver);
  const auto rows = static_cast<Eigen::Index>(prepared.size());
  LinearizedSystem system{Eigen::MatrixXd(rows, 4), Eigen::VectorXd(rows),
                          Eigen::VectorXd(rows)};
  PseudorangeModel model;
  Eigen::Index row = 0;
  for (const auto &measurement : prepared) {
    const auto prediction = model.Evaluate(measurement.observation, receiver,
                                          measurement.satellite,
                                          measurement.corrections);
    system.residuals(row) = measurement.observation.code.pseudorange_m -
                            prediction.predicted_m;
    system.variances_m2(row) = measurement.variance_m2;
    for (Eigen::Index col = 0; col < 3; ++col)
      system.jacobian(row, col) =
          prediction.d_predicted_d_receiver_position[static_cast<size_t>(col)];
    system.jacobian(row, 3) = prediction.d_predicted_d_receiver_clock_bias;
    ++row;
  }
  if (!system.jacobian.allFinite() || !system.residuals.allFinite() ||
      !system.variances_m2.allFinite() ||
      (system.variances_m2.array() <= 0.0).any())
    return std::nullopt;
  return system;
}
