#pragma once

// Precomputed, signed contributions to the predicted code range in metres.
// Zero means the caller has omitted that correction. Satellite clock and
// Earth rotation corrections are computed by the model from the states.
struct CorrectionContext
{
    double troposphere_m{};
    double ionosphere_m{};
    double groupDelay_m{};
};

struct CorrectionBreakdown
{
    double satelliteClock_m{};
    double earthRotation_m{};
    double troposphere_m{};
    double ionosphere_m{};
    double groupDelay_m{};
};
