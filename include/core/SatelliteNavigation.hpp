#pragma once

#include "core/satellite_id.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "navigation/time.hpp"

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

    const NavigationMessage *FindMessage(navigation::GnssTime transmissionTime) const
    {
        const NavigationMessage *best = nullptr;
        navigation::Seconds bestAge{
            std::numeric_limits<navigation::Seconds::rep>::infinity()};

        for (const auto &message : messages)
        {
            const navigation::GnssTime toe = std::visit(
                [](const auto &nav)
                {
                    return nav.ToeEpoch();
                },
                message);

            const navigation::Seconds age{transmissionTime - toe};

            if (age >= navigation::Seconds{0} && age < bestAge)
            {
                best = &message;
                bestAge = age;
            }
        }

        return best;
    }
};
