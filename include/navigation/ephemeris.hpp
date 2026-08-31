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
	navigation::ClockState ClockState;
	navigation::Seconds relativisticCorrection;
	navigation::GnssTime signalTime;
	navigation::GnssTime toe;
	navigation::GnssTime observationTime;
};

template<typename NavigationData, typename OrbitModel, typename ClockModel>
class Ephemeris
{
private: 
	SatelliteState _SatelliteState{};

public:
	// getters
	SatelliteState const &State() const { return this->_SatelliteState; }	

	// functions
	SatelliteState Calculate(const NavigationData& nav, navigation::GnssTime transmitTime) const;

	// ctor & dtor
	Ephemeris() : _SatelliteState{} {};
	virtual ~Ephemeris() = default;
};
