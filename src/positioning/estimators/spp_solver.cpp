#include <Eigen/Dense>
#include <cmath>

#include "positioning/estimators/spp_solver.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

std::vector<SPPState>
SPPSolver::Solve(const std::vector<PositioningEpoch> &epochs) {

  std::vector<SPPState> sppstates;
  sppstates.reserve(epochs.size());

  for (const auto &ep : epochs) {
    ReceiverState rxstate{};
    CorrectionContext context{};

    Eigen::Index rowCount = 0;
    for (const auto &meas : ep.measurements) {
      rowCount +=
          static_cast<Eigen::Index>(meas.observations.CodeObservations.size());
    }

    if (rowCount < 4) {
      sppstates.push_back(
          {.receiverState = std::nullopt, .epochtime = ep.receptionTime});

      // Record insufficient observations for this epoch.
      continue;
    }

    Eigen::MatrixXd H(rowCount, 4);
    Eigen::VectorXd residuals(rowCount);

    constexpr int maxIterations = 20;
    constexpr double tolerance_m = 1e-3;
    bool converged = false;
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
      Eigen::Index i = 0;

      for (const auto &meas : ep.measurements) {
        for (const auto &codeObs : meas.observations.CodeObservations) {
          CodeObservation obs{.satellite = meas.observations.satellite,
                              .band = codeObs.first,
                              .attribute =
                                  ObservationAttribute::Channel_Unknown,
                              .pseudorange_m = codeObs.second.pseudorange_m,
                              .variance_m2 = 0.0};

          PseudorangeModel model;
          PseudorangePrediction pred =
              model.Evaluate(obs, rxstate, meas.satelliteState, context);

          residuals(i) = obs.pseudorange_m - pred.predicted_m;

          H(i, 0) = pred.d_predicted_d_receiver_position[0];
          H(i, 1) = pred.d_predicted_d_receiver_position[1];
          H(i, 2) = pred.d_predicted_d_receiver_position[2];
          H(i, 3) = pred.d_predicted_d_receiver_clock_bias;

          ++i;
        }
      }

      if (!H.allFinite() || !residuals.allFinite())
        break;

      auto qr = H.colPivHouseholderQr();
      if (qr.rank() < 4)
        break;

      Eigen::Vector4d dx = qr.solve(residuals);
      if (!dx.allFinite())
        break;

      rxstate.position = ECEF_Position{rxstate.position.x() + dx(0),
                                       rxstate.position.y() + dx(1),
                                       rxstate.position.z() + dx(2)};
      rxstate.clockBias_m += dx(3);

      if (dx.head<3>().norm() < tolerance_m && std::abs(dx(3)) < tolerance_m) {
        converged = true;
        break;
      }
    }

    // store result
    sppstates.push_back(
        {.receiverState =
             converged ? std::optional<ReceiverState>{rxstate} : std::nullopt,
         .epochtime = ep.receptionTime});
  }

  return sppstates;
}
