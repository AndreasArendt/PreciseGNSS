#include "rinex/NavData/Galileo/GalileoNavData.hpp"

#include <chrono>
#include "navigation/time.hpp"

using namespace std::chrono_literals;

GalileoNavData::GalileoNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second)
{
}

GalileoNavData::~GalileoNavData()
{
}

void GalileoNavData::AddClockErrors(double data0, double data1, double data2)
{
	this->_ClockState = navigation::ClockState {
		.bias = navigation::Seconds{data0},
		.drift = data1,

		// RINEX stores the quadratic coefficient af2; ClockState stores the second derivative
		.driftRate = data2 * 2
	};
}

void GalileoNavData::AddOrbit_1(double data0, double data1, double data2, double data3)
{
	_IODE_IssueOfData = data0;
	_Crs__m = data1;
	_DeltaN__radDs = data2;
	_M0__rad = data3;
}

void GalileoNavData::AddOrbit_2(double data0, double data1, double data2, double data3)
{
	_Cuc__rad = data0;
	_Eccentricity = data1;
	_Cus__rad = data2;
	_SqrtA___sqrtm = data3;
}

void GalileoNavData::AddOrbit_3(double data0, double data1, double data2, double data3)
{
	_toe = navigation::Seconds{data0};
	_Cic__rad = data1;
	_Omega0__rad = data2; // OMEGA_0
	_Cis__rad = data3;
}

void GalileoNavData::AddOrbit_4(double data0, double data1, double data2, double data3)
{
	_i0__rad = data0;
	_Crc__m = data1;
	_Omega__rad = data2;	   // omega
	_Omega_dot__radDs = data3; // OMEGA_DOT
}

void GalileoNavData::AddOrbit_5(double data0, double data1, double data2, [[maybe_unused]] double data3)
{
	_Idot__radDs = data0;
	_DataSources = data1;
	_GalWeek = std::chrono::weeks{static_cast<std::chrono::weeks::rep>(data2)};
	_Spare0 = 0.0;
}

void GalileoNavData::AddOrbit_6(double data0, double data1, double data2, double data3)
{
	_SigAccuracy__m = data0;
	_SvHealth = data1;
	_BGD_E5a_E1 = data2;
	_BGD_E5b_E1 = data3;
}

void GalileoNavData::AddOrbit_7(double data0, [[maybe_unused]] double data1, [[maybe_unused]] double data2, [[maybe_unused]] double data3)
{
	_TransmissiontimeOfMessage = data0;
	_Spare1 = 0.0;
	_Spare2 = 0.0;
	_Spare3 = 0.0;
}

navigation::GalileoTime GalileoNavData::getGST() const
{
	constexpr auto gstEpochOffset = std::chrono::weeks{1024};
	return navigation::GalileoTime{std::chrono::duration_cast<navigation::GalileoClock::duration>(
		_GalWeek + _toe - gstEpochOffset)};
}
