#pragma once

#include "rinex/NavData/NavData.hpp"
#include "navigation/clock.hpp"
#include "navigation/kepler_orbit.hpp"

class GpsNavData : public NavData
{
private:
	KeplerOrbitData _orbit{};
    navigation::PolynomialClockData _clock{};
		
	double _IODE_IssueOfData = 0.0;	
	double _CodesOnL2 = 0.0;
	std::chrono::weeks _GpsWeek{}; // continuous, not Mod(1024)!	
	double _L2P_DataFlag = 0.0;
	
	double _SvAccuracy__m = 0.0;
	double _SvHealth = 0.0;
	double _TGD__s = 0.0;
	double _IODC = 0.0;

	double _TransmissiontimeOfMessage = 0.0;
	double _FitInterval__hrs = 0.0;
	double _Spare0 = 0.0;
	double _Spare1 = 0.0;

protected:
	void AddOrbit_1(double data0, double data1, double data2, double data3);
	void AddOrbit_2(double data0, double data1, double data2, double data3);
	void AddOrbit_3(double data0, double data1, double data2, double data3);
	void AddOrbit_4(double data0, double data1, double data2, double data3);
	void AddOrbit_5(double data0, double data1, double data2, double data3);
	void AddOrbit_6(double data0, double data1, double data2, double data3);
	void AddOrbit_7(double data0, double data1, double data2, double data3);

public:
	// Orbit & Clock
	const KeplerOrbitData& Orbit() const { return _orbit; }
    const navigation::PolynomialClockData& Clock() const { return _clock; }

	// Auxiliary Data
	double const &IODE_IssueOfData() const { return _IODE_IssueOfData; }	
	double const &CodesOnL2() const { return _CodesOnL2; }
	std::chrono::weeks const &GpsWeek() const { return _GpsWeek; }	
	double const &L2P_DataFlag() const { return _L2P_DataFlag; }
	double const &SvAccuracy__m() const { return _SvAccuracy__m; }
	double const &SvHealth() const { return _SvHealth; }
	double const &TGD__s() const { return _TGD__s; }
	double const &IODC() const { return _IODC; }
	double TransmissiontimeOfMessage() const { return _TransmissiontimeOfMessage; } // TOT
	double FitInterval__hrs() const { return _FitInterval__hrs; }
	double Spare0() const { return _Spare0; }
	double Spare1() const { return _Spare1; }

	navigation::GnssTime ToeEpoch() const
	{
		return _orbit.toeEpoch;
	}

	// functions
	void AddClockErrors(double data0, double data1, double data2);

	// ctor & dtor
	GpsNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second) {}
	~GpsNavData() override = default;
};
