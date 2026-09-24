#include <cmath>
#include <future>
#include <gtsam/inference/Symbol.h>
#include <iomanip>
#include <iostream>

#include <gtsam/geometry/Point3.h>
#include "positioning/graph/spp_graph.hpp"

#include "positioning/estimators/spp_solver.hpp"
#include "positioning/providers/broadcast_epoch_provider.hpp"
#include "rinex/RinexParser.hpp"

int main(int argc, char *argv[]) {
  const std::filesystem::path dataPath = RINEX_DATA_DIR;
  const std::filesystem::path observationPath =
      dataPath / "AUBG00DEU_R_20262390000_01D_30S_MO.obs";
  const std::filesystem::path navigationPath =
      dataPath / "AUBG00DEU_R_20262390000_01D_MN.nav";

  RinexParser parser;
  const RinexFile obsResult = parser.Parse(observationPath);
  const RinexFile navResult = parser.Parse(navigationPath);

  const auto *observations = std::get_if<ObservationFile>(&obsResult);
  const auto *navigation = std::get_if<NavigationFile>(&navResult);

  if (!observations) {
    std::cerr << observationPath << " is not an observation file\n";
    return EXIT_FAILURE;
  }

  if (!navigation) {
    std::cerr << navigationPath << " is not a navigation file\n";
    return EXIT_FAILURE;
  }

  BroadcastEpochProvider provider{*navigation};
  std::vector<PositioningEpoch> posEpochs;
  for (const ObservationEpoch &epoch : observations->epochs) {
    PositioningEpoch pEpoch = provider.GetEpoch(epoch);
    posEpochs.emplace_back(pEpoch);
  }

  // Run independently: the graph is not initialized from the LSQ solution.
  auto lsqTask = std::async(std::launch::async,
                            [&] { return SPPSolver{}.Solve(posEpochs); });

  ReceiverState graphInitial;
  graphInitial.position = observations->approximateMarkerPosition;

  auto graphTask = std::async(std::launch::async, [&] {
    std::vector<std::optional<double>> distances(posEpochs.size());
    for (size_t k = 0; k < posEpochs.size(); ++k) {
      try {
        // SPP epochs have no temporal factors; solve each small graph
        // separately.
        const auto values = SppGraph{}.Solve({posEpochs[k]}, {graphInitial});
        const auto positionKey = gtsam::Symbol('x', 0);
        const auto &position = values.at<gtsam::Point3>(positionKey);
        distances[k] =
            (position - observations->approximateMarkerPosition.vector()).norm();
      } catch (const std::exception &error) {
        std::cerr << "Graph epoch " << k << ": " << error.what() << '\n';
      }
    }
    return distances;
  });

  const auto lsqResults = lsqTask.get();
  const auto graphDistances = graphTask.get();
  std::cout << std::fixed << std::setprecision(4)
            << "epoch | lsq [m] | factor [m] (distance to approximate marker)\n";
  for (size_t k = 0; k < posEpochs.size(); ++k) {
    std::cout << k << " | ";
    if (lsqResults[k].receiverState)
      std::cout << (lsqResults[k].receiverState->position -
                    observations->approximateMarkerPosition).norm();
    else
      std::cout << "failed";
    std::cout << " | ";
    if (graphDistances[k])
      std::cout << *graphDistances[k];
    else
      std::cout << "failed";
    std::cout << '\n';
  }

  return EXIT_SUCCESS;
}
