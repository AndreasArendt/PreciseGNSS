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
        ClockDriftRate driftRate{};

        void propagate(Seconds dt)
        {
            const double t = dt.count();

            bias += Seconds{
                drift.secondsPerSecond * t +
                0.5 * driftRate.secondsPerSecondSquared * t * t};

            drift.secondsPerSecond += driftRate.secondsPerSecondSquared * t;
        }
    };
}