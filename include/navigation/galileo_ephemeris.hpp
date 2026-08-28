#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Galileo/GalileoSvHealth.hpp"

class GalileoEphemeris : public Ephemeris<GalileoSvHealth>
{   

private:
	void CalcVelocity();
	void CalcEphemeris(const GalileoNavData& navData, double time, double obstime);
	void CalcClockOffset(const GalileoNavData& navData, double time);
	
public:
	void Calculate(const GalileoNavData& navData, double time, double obstime);

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris() override = default;	
};
