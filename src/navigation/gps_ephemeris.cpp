#include "navigation/gps_ephemeris.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "navigation/kepler_orbit.hpp"

GpsEphemeris::GpsEphemeris(GpsSvHealth svHealth) : Ephemeris<GpsSvHealth>(svHealth)
{
}

void GpsEphemeris::CalcClockOffset(const GpsNavData& nav, navigation::GnssTime signalTime)
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

void GpsEphemeris::CalcEphemeris(const GpsNavData& navData, navigation::GnssTime signalTime, navigation::GnssTime observationTime)
{
	KeplerOrbitData orbitData =
	{
		.SqrtA___sqrtm = navData.SqrtA___sqrtm(),
		.DeltaN__radDs = navData.DeltaN__radDs(),
		.M0__rad = navData.M0__rad(),
		.Eccentricity = navData.Eccentricity(),
		.Omega__rad = navData.Omega__rad(),
		.Omega0__rad = navData.Omega0__rad(),
		.Omega_dot__radDs = navData.Omega_dot__radDs(),
		.Cus__rad = navData.Cus__rad(),
		.Cuc__rad = navData.Cuc__rad(),
		.Crs__m = navData.Crs__m(),
		.Crc__m = navData.Crc__m(),
		.Cis__rad = navData.Cis__rad(),
		.Cic__rad = navData.Cic__rad(),
		.i0__rad = navData.i0__rad(),
		.Idot__radDs = navData.Idot__radDs(),
		.toeEpoch = navData.ToeEpoch(),
		.toe = navData.Toe()
	};

	auto orbit = KeplerOrbit();	
	auto pos_vel = orbit.CalcEphemeris(orbitData, signalTime, observationTime);
	this->_Position_E = std::get<0>(pos_vel);
	this->_Velocity_E = std::get<1>(pos_vel);

	this->_relativisticCorrection = orbit.RelativisticCorrection();
	this->_signalTime = signalTime;
	this->_toe = navData.ToeEpoch();
	this->_observationTime = observationTime;
}

void GpsEphemeris::CalcVelocity()
{

}
