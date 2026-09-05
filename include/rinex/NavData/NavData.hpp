#pragma once

#include "rinex/NavData/ENavOrbitNumber.hpp"
#include "core/epoch.hpp"

class NavData
{
protected:
	virtual void AddOrbit_1(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_2(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_3(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_4(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_5(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_6(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_7(double data0, double data1, double data2, double data3) = 0;
	
	Epoch _Epoch;

public:	
	// functions
	virtual void AddClockErrors(double data0, double data1, double data2) = 0;
	
	void AddOrbit(ENavOrbitNumber orbitNumber, double data0, double data1, double data2, double data3);

	// getters
	Epoch Epoche() const { return this->_Epoch; }
	
	// ctor & dtor
	NavData() = default;
	NavData(int year, int month, int day, int hour, int minute, double second) : _Epoch(year, month, day, hour, minute, second) {}
	virtual ~NavData() = default;	
};

