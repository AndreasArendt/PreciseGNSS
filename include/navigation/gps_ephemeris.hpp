#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "rinex/NavData/Gps/GpsSvHealth.hpp"
#include "navigation/clock.hpp"

class GpsEphemeris : public Ephemeris<GpsSvHealth>
{

private:

public:
	void CalcVelocity();
	void CalcEphemeris(const GpsNavData& navData, navigation::GnssTime signalTime, navigation::GnssTime observationTime);
	void CalcClockOffset(const GpsNavData& navData, navigation::GnssTime signalTime);

	// ctor & dtor	
	GpsEphemeris(GpsSvHealth svHealth);
	~GpsEphemeris() override = default;		
};
