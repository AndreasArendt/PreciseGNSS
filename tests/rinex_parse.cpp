#include <cstdlib>
#include <iostream>
#include <variant>

#include "rinex/RinexParser.hpp"

int main(int argc, char *argv[])
{
    //const char *path = argc > 1 ? argv[1] : "/workspaces/PreciseGnss/data/FLY_0204.obs";
    const char *path = argc > 1 ? argv[1] : "/workspaces/PreciseGnss/data/FLY_0204.nav";
    const RinexFile file = RinexParser{}.Parse(path);

    if (const auto *observations = std::get_if<ObservationFile>(&file))
    {
        std::cout << "Observation epochs: " << observations->epochs.size() << '\n';
    }
    else if (const auto *navigation = std::get_if<NavigationFile>(&file))
    {
        std::size_t messageCount = 0;
        for (const auto &satellite : navigation->satellites)
        {
            messageCount += satellite.messages.size();
        }
        std::cout << "Navigation satellites: " << navigation->satellites.size() << '\n'
                  << "Navigation messages: " << messageCount << '\n';
    }

    return EXIT_SUCCESS;
}
