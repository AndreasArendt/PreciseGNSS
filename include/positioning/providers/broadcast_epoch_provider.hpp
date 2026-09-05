#pragma once

#include "core/ObservationEpoch.hpp"
#include "positioning/positioning_epoch.hpp"
#include "rinex/RinexFile.hpp"

class BroadcastEpochProvider
{
public:
    BroadcastEpochProvider(const NavigationFile &navigation) : _navigation{navigation} {}

    PositioningEpoch GetEpoch(const ObservationEpoch &epoch) const;

private:
    const NavigationFile &_navigation;
};