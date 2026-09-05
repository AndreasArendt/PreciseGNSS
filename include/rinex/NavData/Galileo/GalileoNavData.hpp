#pragma once

#include "rinex/NavData/NavData.hpp"
#include "navigation/clock.hpp"
#include "navigation/kepler_orbit.hpp"

class GalileoNavData : public NavData
{
private:
	// Orbit & Clock
	KeplerOrbitData _orbit{};
	navigation::PolynomialClockData _clock{};

	// Orbit 1
	double _IODE_IssueOfData = 0.0;
	double _DataSources = 0.0;
	std::chrono::weeks _GalWeek{};
	double _Spare0 = 0.0;
	double _SigAccuracy__m = 0.0;
	double _SvHealth = 0.0;
	double _BGD_E5a_E1 = 0.0;
	double _BGD_E5b_E1 = 0.0;
	double _TransmissiontimeOfMessage = 0.0;
	double _Spare1 = 0.0;
	double _Spare2 = 0.0;
	double _Spare3 = 0.0;

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
	const KeplerOrbitData &Orbit() const { return _orbit; }
	const navigation::PolynomialClockData &Clock() const { return _clock; }
	double IODE_IssueOfData() const { return _IODE_IssueOfData; }
	double DataSources() const { return _DataSources; }
	std::chrono::weeks GalWeek() const { return _GalWeek; }
	double Spare0() const { return _Spare0; }
	double SigAccuracy__m() const { return _SigAccuracy__m; }
	double SvHealth() const { return _SvHealth; }
	double BGD_E5a_E1() const { return _BGD_E5a_E1; }
	double BGD_E5b_E1() const { return _BGD_E5b_E1; }
	double TransmissiontimeOfMessage() const { return _TransmissiontimeOfMessage; } // TOT
	double Spare1() const { return _Spare1; }
	double Spare2() const { return _Spare2; }
	double Spare3() const { return _Spare3; }

	navigation::GnssTime ToeEpoch() const
	{
		return _orbit.toeEpoch;
	}

	// functions
	void AddClockErrors(double data0, double data1, double data2);

	GalileoNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second) {}
	~GalileoNavData() override = default;
};
