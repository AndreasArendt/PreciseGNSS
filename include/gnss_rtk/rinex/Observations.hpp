#pragma once

namespace Rinex::Observation
{
    struct Code
    {
        double pseudorange_m{};
    };

    struct Doppler
    {
        double doppler_hz{};
    };

    struct CarrierPhase
    {
        double carrier_phase_cycles{};
    };

    struct SignalStrength
    {
        double snr{};
    };
}
