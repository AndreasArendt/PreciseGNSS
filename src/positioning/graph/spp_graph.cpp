#include "positioning/graph/spp_graph.hpp"

#include "positioning/graph/pseudorange_factor.hpp"
#include "positioning/measurements/prepare_code_epoch.hpp"
#include <gtsam/inference/Symbol.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <stdexcept>

gtsam::Values
SppGraph::Solve(const std::vector<PositioningEpoch> &epochs,
                const std::vector<ReceiverState> &initialStates) const {
  if (epochs.size() != initialStates.size())
    throw std::invalid_argument("Supply one initial receiver state per epoch");
  gtsam::NonlinearFactorGraph graph;
  gtsam::Values initialValues;
  for (size_t k = 0; k < epochs.size(); ++k) {
    const auto &receiver = initialStates[k];
    const auto prepared = PrepareCodeEpoch(epochs[k], receiver);
    const auto positionKey = gtsam::Symbol('x', k);
    const auto clockKey = gtsam::Symbol('c', k);
    initialValues.insert(positionKey,
                         gtsam::Point3(receiver.position.vector()));
    initialValues.insert(clockKey, receiver.clockBias_m);
    for (const auto &measurement : prepared) {
      auto noise =
          gtsam::noiseModel::Isotropic::Variance(1, measurement.variance_m2);
      graph.emplace_shared<PseudorangeFactor>(
          positionKey, clockKey, measurement.observation, measurement.satellite,
          measurement.corrections, noise);
    }
  }
  if (graph.empty())
    return initialValues;
  return gtsam::LevenbergMarquardtOptimizer(graph, initialValues).optimize();
}
