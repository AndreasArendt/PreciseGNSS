#pragma once

#include "gnss_rtk/navigation/ephemeris.hpp"
#include "gnss_rtk/rinex/NavData/Galileo/GalileoSvHealth.hpp"

#include <memory>

class GalileoEphemeris : virtual public Ephemeris
{   

private:
	
public:
	void CalcVelocity();
	void CalcEphemeris(NavData& navData, double time, double obstime) override;
	double CalcClockOffset(NavData& navData, double time) override;
	std::unique_ptr<Ephemeris> clone() const override;

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris();
};
