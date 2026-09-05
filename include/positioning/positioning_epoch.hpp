#pragma once

#include "core/SatelliteObservation.hpp"
#include "navigation/ephemeris.hpp"
#include "navigation/time.hpp"

#include <vector>

struct SatelliteMeasurement
{
    SatelliteObservation observations;
    SatelliteState satelliteState;
    navigation::Seconds approximateTravelTime;
};

struct PositioningEpoch
{
    navigation::GnssTime receptionTime;
    std::vector<SatelliteMeasurement> measurements;
};
