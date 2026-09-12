#include <cmath>
#include <iostream>

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

  SPPSolver solver{};
  std::vector<EpochSolveResult> sppResult = solver.Solve(posEpochs);

  int i = 0;
  for (const auto &spp : sppResult) {
    if (const auto &receiverState = spp.receiverState) {
      double delta = (spp.receiverState->position -
                      observations->approximateMarkerPosition)
                         .norm();
      std::cout << delta << " iter: " << spp.diagnostics.iterations << std::endl;
    } else {
      std::cout << "Failed epoch" << static_cast<int>(spp.status) << std::endl;
    }
    ++i;
  }

  return EXIT_SUCCESS;
}
