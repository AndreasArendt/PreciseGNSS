#include "rinex/NavData/Gps/GpsNavData.hpp"
#include "navigation/clock.hpp"

GpsNavData::GpsNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second)
{
}

GpsNavData::~GpsNavData()
{
}

void GpsNavData::AddClockErrors(double data0, double data1, double data2)
{
	this->_ClockState = navigation::ClockState{
		.bias = navigation::Seconds{data0},
		.drift = data1,

		// RINEX stores the quadratic coefficient af2; ClockState stores the second derivative
		.driftRate = data2 * 2};
}

void GpsNavData::AddOrbit_1(double data0, double data1, double data2, double data3)
{
	_IODE_IssueOfData = data0;
	_Crs__m = data1;
	_DeltaN__radDs = data2;
	_M0__rad = data3;
}

void GpsNavData::AddOrbit_2(double data0, double data1, double data2, double data3)
{
	_Cuc__rad = data0;
	_Eccentricity = data1;
	_Cus__rad = data2;
	_SqrtA___sqrtm = data3;
}

void GpsNavData::AddOrbit_3(double data0, double data1, double data2, double data3)
{
	_toe = navigation::Seconds{data0};
	_Cic__rad = data1;
	_Omega0__rad = data2;
	_Cis__rad = data3;
}

void GpsNavData::AddOrbit_4(double data0, double data1, double data2, double data3)
{
	_i0__rad = data0;
	_Crc__m = data1;
	_Omega__rad = data2;
	_Omega_dot__radDs = data3;
}

void GpsNavData::AddOrbit_5(double data0, double data1, double data2, double data3)
{
	_Idot__radDs = data0;
	_CodesOnL2 = data1;
	_GpsWeek = std::chrono::weeks{static_cast<std::chrono::weeks::rep>(data2)};
	_L2P_DataFlag = data3;
}

void GpsNavData::AddOrbit_6(double data0, double data1, double data2, double data3)
{
	_SvAccuracy__m = data0;
	_SvHealth = data1;
	_TGD__s = data2;
	_IODC = data3;
}

void GpsNavData::AddOrbit_7(double data0, double data1, double data2, double data3)
{
	_TransmissiontimeOfMessage = data0;
	_FitInterval__hrs = data1;
	_Spare0 = data2;
	_Spare1 = data3;
}
