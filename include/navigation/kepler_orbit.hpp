#pragma once

#include "rinex/NavData/NavData.hpp"
#include "coordinates/ecef_position.hpp"
#include "coordinates/ecef_velocity.hpp"
#include "navigation/time.hpp"

struct KeplerOrbitData
{
	double SqrtA___sqrtm;	
	double DeltaN__radDs;
	double M0__rad;
	double Eccentricity;
	double Omega__rad;
	double Omega0__rad;
	double Omega_dot__radDs;
	double Cus__rad;
	double Cuc__rad;
	double Crs__m;
	double Crc__m;
	double Cis__rad;
	double Cic__rad;
	double i0__rad;
	double Idot__radDs;
	navigation::GnssTime toeEpoch;
	navigation::Seconds toe;
};

class KeplerOrbit
{
private:	
	navigation::Seconds _relativisticCorrection;

	double CalcMeanAnomaly(KeplerOrbitData& orbitData, navigation::GnssTime signalTime);

public:
	std::tuple<ECEF_Position, ECEF_Velocity> CalcEphemeris(KeplerOrbitData& orbitData, navigation::GnssTime signalTime, navigation::GnssTime observationTime);
	navigation::Seconds const& RelativisticCorrection() const { return this->_relativisticCorrection; }
};
