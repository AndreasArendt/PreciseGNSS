#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Galileo/GalileoSvHealth.hpp"

class GalileoEphemeris : public Ephemeris<GalileoSvHealth>
{   

private:
	
public:
	void CalcVelocity();
	void CalcEphemeris(const GalileoNavData& navData, double time, double obstime);
	double CalcClockOffset(const GalileoNavData& navData, double time);

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris() override = default;	
};
