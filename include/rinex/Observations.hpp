#pragma once

#include <optional>

namespace Rinex::Observation
{
    struct QualityIndicators
    {
        // Raw RINEX LLI bit mask; meaningful for carrier phase.
        std::optional<unsigned int> lli;
        // Coarse signal strength indicator, distinct from Sxx observations.
        std::optional<unsigned int> ssi;
    };

    struct Code
    {
        double pseudorange_m{};
        QualityIndicators quality{};
    };

    struct Doppler
    {
        double doppler_hz{};
        QualityIndicators quality{};
    };

    struct CarrierPhase
    {
        double carrier_phase_cycles{};
        QualityIndicators quality{};
    };

    struct SignalStrength
    {
        double snr{};
        QualityIndicators quality{};
    };
}
