#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"

namespace navigation
{
    template <typename NavigationData>
    struct EphemerisTraits;

    template <>
    struct EphemerisTraits<GpsNavData>
    {
        using OrbitModel = KeplerOrbit;
        using ClockModel = PolynomialClock;
    };

    template <>
    struct EphemerisTraits<GalileoNavData>
    {
        using OrbitModel = KeplerOrbit;
        using ClockModel = PolynomialClock;
    };

    template <typename NavigationData>
    SatelliteState CalculateEphemeris(const NavigationData &nav, GnssTime transmissionTime)
    {
        using Traits = EphemerisTraits<NavigationData>;
        using Calculator = Ephemeris<typename Traits::OrbitModel,typename Traits::ClockModel>;

        return Calculator{}.Calculate(nav.Orbit(), nav.Clock(), transmissionTime);
    }
}
