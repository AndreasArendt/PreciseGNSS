#pragma once

#include "coordinates/ecef_position.hpp"
#include "coordinates/ecef_velocity.hpp"
#include "navigation/time.hpp"
#include "navigation/clock.hpp"

template <typename SvHealth>
class Ephemeris
{
protected:
	// remember to add variables to == Operator overload
	ECEF_Position _Position_E;
	ECEF_Velocity _Velocity_E;
	SvHealth _SvHealth;
	navigation::ClockState _ClockState;
	navigation::Seconds _relativisticCorrection;
	navigation::GnssTime _signalTime;
	navigation::GnssTime _toe;
	navigation::GnssTime _observationTime;

public:
	// getters
	ECEF_Position const &Position_E() const { return this->_Position_E; }
	ECEF_Velocity const &Velocity_E() const { return this->_Velocity_E; }
	navigation::ClockState ClockState() const { return this->_ClockState; }
	navigation::Seconds RelativisticCorrection() const { return this->_relativisticCorrection; }
	navigation::GnssTime SignalTime() const { return this->_signalTime; }
	navigation::GnssTime Toe() const { return this->_toe; }
	navigation::GnssTime ObservationTime() const { return this->_observationTime; }
	SvHealth const &SatelliteHealth() const { return this->_SvHealth; }

	// ctor & dtor
	Ephemeris(SvHealth svHealth) : _Position_E{}, _Velocity_E{}, _SvHealth(svHealth), _ClockState{} {};
	virtual ~Ephemeris() = default;
};
