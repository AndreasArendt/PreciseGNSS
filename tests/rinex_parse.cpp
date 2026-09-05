#include "rinex/RinexParser.hpp"
#include "positioning/providers/broadcast_epoch_provider.hpp"

int main(int argc, char *argv[])
{
    const std::filesystem::path observationPath = "/workspaces/PreciseGnss/data/AUBG00DEU_R_20262390000_01D_30S_MO.obs";
    const std::filesystem::path navigationPath = "/workspaces/PreciseGnss/data/AUBG00DEU_R_20262390000_01D_MN.nav";

    RinexParser parser;
    const RinexFile obsResult = parser.Parse(observationPath);
    const RinexFile navResult = parser.Parse(navigationPath);

    const auto *observations = std::get_if<ObservationFile>(&obsResult);
    const auto *navigation = std::get_if<NavigationFile>(&navResult);

    if (!observations)
    {
        std::cerr << observationPath << " is not an observation file\n";
        return EXIT_FAILURE;
    }

    if (!navigation)
    {
        std::cerr << navigationPath << " is not a navigation file\n";
        return EXIT_FAILURE;
    }

    BroadcastEpochProvider provider{*navigation};
    for (const ObservationEpoch &epoch : observations->epochs)
    {        
        auto epoch_measurements = provider.GetEpoch(epoch);
    }

    return EXIT_SUCCESS;
}
