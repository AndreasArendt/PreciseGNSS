#pragma once

#include "time.hpp"

namespace navigation
{
    struct ClockDrift
    {
        double secondsPerSecond{};
    };

    struct ClockDriftRate
    {
        double secondsPerSecondSquared{};
    };

    struct ClockState
    {
        Seconds bias{};
        ClockDrift drift{};
    };

    struct PolynomialClockData
    {
        Seconds bias{};
        ClockDrift drift{};
        ClockDriftRate driftRate{};
        GnssTime referenceTime{};
    };

    class PolynomialClock
    {
    public:
        using Data = PolynomialClockData;

        ClockState Propagate(const Data &clock, GnssTime signalTime) const
        {
            const navigation::Seconds dt{signalTime - clock.referenceTime};
            const double seconds = dt.count();

            return ClockState{
                .bias = clock.bias + navigation::Seconds{
                                         clock.drift.secondsPerSecond * seconds +
                                         clock.driftRate.secondsPerSecondSquared *
                                             seconds * seconds},
                .drift = navigation::ClockDrift{clock.drift.secondsPerSecond + 2.0 * clock.driftRate.secondsPerSecondSquared * seconds},
            };
        }
    };
}
