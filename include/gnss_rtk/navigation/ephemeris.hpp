#pragma once

#include "gnss_rtk/coordinates/ecef_position.hpp"
#include "gnss_rtk/coordinates/ecef_velocity.hpp"

template<typename SvHealth>
class Ephemeris
{
protected:
	// remember to add variables to == Operator overload
	ECEF_Position _Position_E;
	ECEF_Velocity _Velocity_E;
	SvHealth _SvHealth;	
	double _SatelliteClockError__s;
	double _RelativisticError__s;
	double _SatelliteClockDrift__1Ds;
	double _Utc__s;
	double _Toe__s;
	double _Obstime__s;

public:
	// getters	
	ECEF_Position const& Position_E() const { return this->_Position_E; }
	ECEF_Velocity const& Velocity_E() const { return this->_Velocity_E; }
	double SatelliteClockError__s() const { return this->_SatelliteClockError__s;  }
	double SatelliteClockDrift__1Ds() const { return this->_SatelliteClockDrift__1Ds; }
	double RelativisticError__s() const { return this->_RelativisticError__s; }
	double Utc() const { return this->_Utc__s; }
	double Toe__s() const { return this->_Toe__s; }
	double Obstime__s() const { return this->_Obstime__s; }
	SvHealth const& SatelliteHealth() const { return this->_SvHealth; }

	// cotr & dtor
	Ephemeris(SvHealth svHealth) : _Position_E{}, _Velocity_E{}, _SvHealth(svHealth), _SatelliteClockError__s(0), _RelativisticError__s(0), _SatelliteClockDrift__1Ds(0),  _Utc__s(0), _Toe__s(0), _Obstime__s(0)	{};
	virtual ~Ephemeris() = default;
};
