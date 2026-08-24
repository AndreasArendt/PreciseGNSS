#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "rinex/NavData/Gps/GpsSvHealth.hpp"

class GpsEphemeris : public Ephemeris<GpsSvHealth>
{

private:

public:
	void CalcVelocity();
	void CalcEphemeris(const GpsNavData& navData, double time, double obstime);
	double CalcClockOffset(const GpsNavData& navData, double time);

	// ctor & dtor	
	GpsEphemeris(GpsSvHealth svHealth);
	~GpsEphemeris() override = default;		
};
