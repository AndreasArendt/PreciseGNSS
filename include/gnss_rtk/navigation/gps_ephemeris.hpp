#pragma once

#include "gnss_rtk/navigation/ephemeris.hpp"
#include "gnss_rtk/rinex/NavData/Gps/GpsNavData.hpp"
#include "gnss_rtk/rinex/NavData/Gps/GpsSvHealth.hpp"

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
