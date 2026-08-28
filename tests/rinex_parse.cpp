#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <variant>
#include <chrono>

#include "coordinates/transformation.hpp"
#include "rinex/RinexParser.hpp"
#include "navigation/time.hpp"

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

    std::size_t matchedObservations = 0;
    std::size_t missingSatellites = 0;
    std::size_t missingMessages = 0;

    for (const ObservationEpoch &epoch : observations->epochs)
    {
        const navigation::GnssTime receptionTime = epoch.time.Time();

        for (const SatelliteObservation &observation : epoch.satellites)
        {                        
            // find obs satellite in nav
            const auto navSatellite = std::ranges::find(navigation->satellites, observation.satellite, &SatelliteNavigation::satellite);
                   
            // did not find any
            if(navSatellite == navigation->satellites.end())                
            {
                ++missingSatellites;
                continue;
            }

            if (observation.CodeObservations.empty())
            {
                continue;
            }

            const double pseudorange = observation.CodeObservations.begin()->second.pseudorange_m;

            const auto signalTravelTime = navigation::Seconds {pseudorange / Transformation::SpeedOfLight__mDs};
            const auto transmissionTime = navigation::GnssTime{
                std::chrono::round<navigation::GnssClock::duration>(receptionTime - signalTravelTime)};

            const NavigationMessage *message = navSatellite->FindMessage(transmissionTime);

            if (!message)
            {
                ++missingMessages;
                continue;
            }

            ++matchedObservations;
        }
    }

    std::cout << "Observation epochs: " << observations->epochs.size() << '\n'
              << "Navigation satellites: " << navigation->satellites.size() << '\n'
              << "Matched observations: " << matchedObservations << '\n'
              << "Missing satellites: " << missingSatellites << '\n'
              << "Missing navigation messages: " << missingMessages << '\n';

    return EXIT_SUCCESS;
}
