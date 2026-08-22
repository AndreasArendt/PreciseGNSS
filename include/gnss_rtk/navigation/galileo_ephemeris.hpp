#pragma once

#include "gnss_rtk/navigation/ephemeris.hpp"
#include "gnss_rtk/rinex/NavData/Galileo/GalileoNavData.hpp"
#include "gnss_rtk/rinex/NavData/Galileo/GalileoSvHealth.hpp"

class GalileoEphemeris : public Ephemeris
{   

private:
	
public:
	void CalcVelocity();
	void CalcEphemeris(const GalileoNavData& navData, double time, double obstime);
	double CalcClockOffset(const GalileoNavData& navData, double time);

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris() = default;
};
