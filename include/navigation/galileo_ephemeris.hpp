#pragma once

#include "navigation/ephemeris.hpp"
#include "rinex/NavData/Galileo/GalileoNavData.hpp"
#include "rinex/NavData/Galileo/GalileoSvHealth.hpp"

class GalileoEphemeris : public Ephemeris<GalileoSvHealth>
{   

private:
	void CalcVelocity();
	void CalcEphemeris(const GalileoNavData& navData, navigation::GnssTime signalTime, navigation::GnssTime observationTime);
	void CalcClockOffset(const GalileoNavData& navData, navigation::GnssTime signalTime);
	
public:
	void Calculate(const GalileoNavData& navData, navigation::GnssTime signalTime, navigation::GnssTime observationTime);

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris() override = default;	
};
