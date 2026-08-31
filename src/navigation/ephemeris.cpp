#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"

template <typename NavigationData, typename OrbitModel, typename ClockModel>
SatelliteState Ephemeris<NavigationData, OrbitModel, ClockModel>::Calculate(const NavigationData &nav, navigation::GnssTime transmitTime) const
{
    const KeplerOrbitData orbitData{
        nav.SqrtA___sqrtm(), nav.DeltaN__radDs(), nav.M0__rad(), nav.Eccentricity(),
        nav.Omega__rad(), nav.Omega0__rad(), nav.Omega_dot__radDs(), nav.Cus__rad(),
        nav.Cuc__rad(), nav.Crs__m(), nav.Crc__m(), nav.Cis__rad(), nav.Cic__rad(),
        nav.i0__rad(), nav.Idot__radDs(), nav.ToeEpoch(), nav.Toe()};
    OrbitModel orbitModel{};
    SatelliteState state{};
    navigation::GnssTime signalTime = transmitTime;

    constexpr navigation::Seconds tolerance{1e-11}; // 10 ps
    constexpr int maxIterations = 8;

    for (int iteration = 0; iteration < maxIterations; ++iteration)
    {
        state.ClockState = nav.ClockState();
        state.ClockState.Propagate(navigation::Seconds{signalTime - nav.Epoche().Time()});
        const auto orbitState = orbitModel.Propagate(orbitData, signalTime);
        const navigation::Seconds correction = state.ClockState.bias + orbitState.relativisticCorrection;
        const navigation::GnssTime nextSignalTime = transmitTime - std::chrono::round<navigation::GnssClock::duration>(correction);

        const navigation::Seconds change{nextSignalTime - signalTime};
        signalTime = nextSignalTime;

        if (std::abs(change.count()) <= tolerance.count())
            break;
    }

    const auto orbitState = orbitModel.Propagate(orbitData, signalTime);
    state.Position_E = orbitState.position;
    state.Velocity_E = orbitState.velocity;
    state.relativisticCorrection = orbitState.relativisticCorrection;
    state.signalTime = signalTime;
    state.toe = nav.ToeEpoch();
    state.observationTime = transmitTime;
    return state;
}

template class Ephemeris<GpsNavData, KeplerOrbit, navigation::ClockState>;
