#pragma once

#include "core/satellite_id.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"

#include <limits>
#include <variant>
#include <vector>

using NavigationMessage = std::variant<
    GpsNavData,
    GalileoNavData>;

struct SatelliteNavigation
{
    SatelliteId satellite;
    std::vector<NavigationMessage> messages;

    const NavigationMessage *FindMessage(double transmissionTime) const
    {
        const NavigationMessage *best = nullptr;
        double bestAge = std::numeric_limits<double>::infinity();

        for (const auto &message : messages)
        {
            const double toe = std::visit(
                [](const auto &nav)
                {
                    return nav.ToeEpoch();
                },
                message);

            const double age = transmissionTime - toe;

            if (age >= 0.0 && age < bestAge)
            {
                best = &message;
                bestAge = age;
            }
        }

        return best;
    }
};
