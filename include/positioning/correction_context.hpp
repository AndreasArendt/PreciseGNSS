#pragma once

struct CorrectionBreakdown
{
    double satelliteClock_m{};
    double earthRotation_m{};
    double troposphere_m{};
    double ionosphere_m{};
    double groupDelay_m{};
};