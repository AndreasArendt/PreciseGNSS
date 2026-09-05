#pragma once

#include "coordinates/ecef_position.hpp"
#include "coordinates/ecef_velocity.hpp"
#include "navigation/time.hpp"
#include "navigation/clock.hpp"
#include "navigation/kepler_orbit.hpp"

struct SatelliteState
{
	ECEF_Position Position_E;
	ECEF_Velocity Velocity_E;
	navigation::ClockState clock;
	navigation::Seconds relativisticCorrection;
	navigation::GnssTime signalTime;	
};

template <typename OrbitModel, typename ClockModel>
class Ephemeris
{
public:
	// functions
	SatelliteState Calculate(const typename OrbitModel::Data &orbit,
							 const typename ClockModel::Data &clock,
							 navigation::GnssTime transmitTime) const;

	// ctor & dtor
	Ephemeris() {};
	virtual ~Ephemeris() = default;
};

template <typename OrbitModel, typename ClockModel>
SatelliteState Ephemeris<OrbitModel, ClockModel>::Calculate(
	const typename OrbitModel::Data &orbit,
	const typename ClockModel::Data &clock,
	navigation::GnssTime transmitTime) const
{
	OrbitModel orbitModel{};
	ClockModel clockModel{};
	SatelliteState state{};
	navigation::GnssTime signalTime = transmitTime;

	constexpr navigation::Seconds tolerance{1e-11}; // 10 ps
	constexpr int maxIterations = 8;

	for (int iteration = 0; iteration < maxIterations; ++iteration)
	{
		state.clock = clockModel.Propagate(clock, signalTime);
		const auto orbitState = orbitModel.Propagate(orbit, signalTime);
		const navigation::Seconds correction = state.clock.bias + orbitState.relativisticCorrection;
		const navigation::GnssTime nextSignalTime = transmitTime - std::chrono::round<navigation::GnssClock::duration>(correction);

		const navigation::Seconds change{nextSignalTime - signalTime};
		signalTime = nextSignalTime;

		if (std::abs(change.count()) <= tolerance.count())
			break;
	}

	state.clock = clockModel.Propagate(clock, signalTime);
	const auto orbitState = orbitModel.Propagate(orbit, signalTime);
	state.Position_E = orbitState.position;
	state.Velocity_E = orbitState.velocity;
	state.relativisticCorrection = orbitState.relativisticCorrection;
	state.signalTime = signalTime;
	return state;
}