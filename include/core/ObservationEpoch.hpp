#pragma once

#include "core/epoch.hpp"
#include "core/SatelliteObservation.hpp"

struct ObservationEpoch {
    Epoch time;
    int flag;
    std::vector<SatelliteObservation> satellites;
};