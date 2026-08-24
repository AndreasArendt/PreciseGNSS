#include "rinex/NavData/Galileo/GalileoNavData.hpp"

#include <chrono>

GalileoNavData::GalileoNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second)
{
}

GalileoNavData::~GalileoNavData()
{
}

void GalileoNavData::AddClockErrors(double data0, double data1, double data2)
{
	_SV_ClockBias__s = data0;
	_SV_ClockDrift__sDs = data1;
	_SV_ClockDriftRate__sDs2 = data2;
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
	_Toe__s = Seconds{data0};
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

double GalileoNavData::getGST() const
{
	constexpr auto gstEpochOffset = std::chrono::weeks{1024};
	const auto gst = _GalWeek + _Toe__s - gstEpochOffset;
	return std::chrono::duration_cast<Seconds>(gst).count();
}

double GalileoNavData::ToeEpoch() const
{
	using Seconds = std::chrono::duration<double>;
	using TimePoint = std::chrono::sys_time<Seconds>;

	constexpr auto gpsEpoch = std::chrono::sys_days{std::chrono::year{1980} / std::chrono::January / 6};
	constexpr auto week = std::chrono::weeks{1};
	constexpr auto halfWeek = std::chrono::days{3} + std::chrono::hours{12};

	auto toe = TimePoint{gpsEpoch.time_since_epoch()} + this->_GalWeek + this->_Toe__s;

	const auto epoch = TimePoint{Seconds{this->_Epoch.PosixEpochTime__s()}};
	const auto difference = toe - epoch;

	if (difference < -halfWeek)
	{
		toe += week;
	}
	else if (difference > halfWeek)
	{
		toe -= week;
	}

	return toe.time_since_epoch().count();
}
