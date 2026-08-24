#pragma once

#include "rinex/NavData/NavData.hpp"
#include "rinex/NavData/Galileo/GalileoSvHealth.hpp"

#include <chrono>

class GalileoNavData : virtual public NavData
{
private:
	using Seconds = std::chrono::duration<double>;	
	
	// Clock Data
	double _SV_ClockBias__s = 0.0;
	double _SV_ClockDrift__sDs = 0.0;
	double _SV_ClockDriftRate__sDs2 = 0.0;

	//Orbit 1
	double _IODE_IssueOfData = 0.0;
	double _Crs__m = 0.0;
	double _DeltaN__radDs = 0.0;
	double _M0__rad = 0.0;

	// Orbit 2
	double _Cuc__rad = 0.0;
	double _Eccentricity = 0.0;
	double _Cus__rad = 0.0;
	double _SqrtA___sqrtm = 0.0;
	
	// Orbit 3	
	Seconds _Toe__s{}; //Time of Ephemeris
	double _Cic__rad = 0.0;
	double _Omega0__rad = 0.0;
	double _Cis__rad = 0.0;

	// Orbit 4
	double _i0__rad = 0.0;
	double _Crc__m = 0.0;
	double _Omega__rad = 0.0;
	double _Omega_dot__radDs = 0.0;

	// Orbit 5
	double _Idot__radDs = 0.0;
	double _DataSources = 0.0;	
	std::chrono::weeks _GalWeek{};
	double _Spare0 = 0.0;

	// Orbit 6
	double _SigAccuracy__m = 0.0;
	double _SvHealth = 0.0;
	double _BGD_E5a_E1 = 0.0;
	double _BGD_E5b_E1 = 0.0;

	// Orbit 7		
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
	// Clock Data
	double SV_ClockBias__s() const { return _SV_ClockBias__s; }
	double SV_ClockDrift__sDs() const { return _SV_ClockDrift__sDs; }
	double SV_ClockDriftRate__sDs2() const { return _SV_ClockDriftRate__sDs2; }

	// Orbit 1
	double IODE_IssueOfData() const { return _IODE_IssueOfData; }
	double Crs__m() const { return _Crs__m; }
	double DeltaN__radDs() const { return _DeltaN__radDs; }
	double M0__rad() const { return _M0__rad; }

	// Orbit 2
	double Cuc__rad() const { return _Cuc__rad; }
	double Eccentricity() const { return _Eccentricity; }
	double Cus__rad() const { return _Cus__rad; }
	double SqrtA___sqrtm() const { return _SqrtA___sqrtm; }

	// Orbit 3
	double Toe__s() const { return _Toe__s.count(); }
	double Cic__rad() const { return _Cic__rad; }
	double Omega0__rad() const { return _Omega0__rad; }
	double Cis__rad() const { return _Cis__rad; }

	// Orbit 4
	double i0__rad() const { return _i0__rad; }
	double Crc__m() const { return _Crc__m; }
	double Omega__rad() const { return _Omega__rad; }
	double Omega_dot__radDs() const { return _Omega_dot__radDs; }

	// Orbit 5
	double Idot__radDs() const { return _Idot__radDs; }
	double DataSources() const { return _DataSources; }
	long int GalWeek() const { return _GalWeek.count(); }
	double Spare0() const { return _Spare0; }
	
	// Orbit 6
	double SigAccuracy__m() const { return _SigAccuracy__m; }
	double SvHealth() const { return _SvHealth; }
	double BGD_E5a_E1() const { return _BGD_E5a_E1; }
	double BGD_E5b_E1() const { return _BGD_E5b_E1; }

	// Orbit 7
	double TransmissiontimeOfMessage() const { return _TransmissiontimeOfMessage; } //TOT
	double Spare1() const { return _Spare1; }
	double Spare2() const { return _Spare2; }
	double Spare3() const { return _Spare3; }

	// functions
	void AddClockErrors(double data0, double data1, double data2);	
	double getGST() const;
	double ToeEpoch() const;

	GalileoNavData(int year, int month, int day, int hour, int minute, double second);
	~GalileoNavData();
};
