#include "navigation/galileo_ephemeris.hpp"
#include "coordinates/transformation.hpp"
#include "navigation/kepler_orbit.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"

#include <cmath>
#include <chrono>

GalileoEphemeris::GalileoEphemeris(GalileoSvHealth svHealth) : Ephemeris<GalileoSvHealth>(svHealth)
{
}

void GalileoEphemeris::CalcClockOffset(const GalileoNavData& nav, navigation::GnssTime signalTime)
{
	const navigation::ClockState reference = nav.ClockState();
	const navigation::Seconds elapsed = navigation::Seconds{signalTime - nav.Epoche().Time()};

	navigation::Seconds correctedElapsed = elapsed;
	for (int i = 0; i < 2; ++i)
	{
		navigation::ClockState clock = reference;
		clock.propagate(correctedElapsed);
		correctedElapsed = elapsed - clock.bias;
	}

	this->_ClockState = reference;
	this->_ClockState.propagate(correctedElapsed);
}

void GalileoEphemeris::CalcEphemeris(const GalileoNavData& nav, navigation::GnssTime signalTime, navigation::GnssTime observationTime)
{
	KeplerOrbitData orbitData =
	{
		.SqrtA___sqrtm = nav.SqrtA___sqrtm(),
		.DeltaN__radDs = nav.DeltaN__radDs(),
		.M0__rad = nav.M0__rad(),
		.Eccentricity = nav.Eccentricity(),
		.Omega__rad = nav.Omega__rad(),
		.Omega0__rad = nav.Omega0__rad(),
		.Omega_dot__radDs = nav.Omega_dot__radDs(),
		.Cus__rad = nav.Cus__rad(),
		.Cuc__rad = nav.Cuc__rad(),
		.Crs__m = nav.Crs__m(),
		.Crc__m = nav.Crc__m(),
		.Cis__rad = nav.Cis__rad(),
		.Cic__rad = nav.Cic__rad(),
		.i0__rad = nav.i0__rad(),
		.Idot__radDs = nav.Idot__radDs(),
		.toeEpoch = nav.ToeEpoch(),
		.toe = nav.Toe()
	};

	auto orbit = KeplerOrbit();
	auto pos_vel = orbit.CalcEphemeris(orbitData, signalTime, observationTime);
	this->_Position_E = std::get<0>(pos_vel);
	this->_Velocity_E = std::get<1>(pos_vel);
	
	this->_relativisticCorrection = orbit.RelativisticCorrection();
	this->_signalTime = signalTime;
	this->_toe = nav.ToeEpoch();
	this->_observationTime = observationTime;
}

void GalileoEphemeris::CalcVelocity()
{

}

void GalileoEphemeris::Calculate(const GalileoNavData& navData, navigation::GnssTime signalTime, navigation::GnssTime observationTime)
{
	CalcClockOffset(navData, signalTime);
	signalTime -= std::chrono::round<navigation::GnssClock::duration>(this->ClockState().bias);
	CalcEphemeris(navData, signalTime, observationTime);
}
