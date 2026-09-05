#pragma once

#include "core/satellite_id.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "navigation/time.hpp"

#include <chrono>
#include <utility>
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
        constexpr std::chrono::hours maximumAge{4};

        const NavigationMessage *best = nullptr;
        navigation::Seconds bestAge = maximumAge;

        for (const auto &message : messages)
        {
            const auto [toe, messageTime] = std::visit(
                [](const auto &nav)
                {
                    return std::pair{
                        nav.ToeEpoch(),
                        nav.Epoche().Time()};
                },
                message);

            // Do not use a navigation record that had not been broadcast yet.
            if (messageTime > transmissionTime)
                continue;

            const navigation::Seconds age = std::chrono::abs(
                navigation::Seconds{transmissionTime - toe});

            if (age <= maximumAge && (!best || age < bestAge))
            {
                best = &message;
                bestAge = age;
            }
        }

        return best;
    }
};
