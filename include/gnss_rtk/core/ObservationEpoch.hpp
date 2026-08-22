#pragma once

#include "gnss_rtk/core/epoch.hpp"
#include "gnss_rtk/core/SatelliteObservation.hpp"

struct ObservationEpoch {
    Epoch time;
    int flag;
    std::vector<SatelliteObservation> satellites;
};