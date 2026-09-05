#include "rinex/NavData/Galileo/GalileoNavData.hpp"

void GalileoNavData::AddClockErrors(double data0, double data1, double data2)
{
	this->_clock = navigation::PolynomialClockData{
		.bias = navigation::Seconds{data0},
		.drift = data1,
		.driftRate = data2,
		.referenceTime = _Epoch.Time()};
}

void GalileoNavData::AddOrbit_1(double data0, double data1, double data2, double data3)
{
	_IODE_IssueOfData = data0;
	this->_orbit.Crs__m = data1;
	this->_orbit.DeltaN__radDs = data2;
	this->_orbit.M0__rad = data3;
}

void GalileoNavData::AddOrbit_2(double data0, double data1, double data2, double data3)
{
	this->_orbit.Cuc__rad = data0;
	this->_orbit.Eccentricity = data1;
	this->_orbit.Cus__rad = data2;
	this->_orbit.SqrtA___sqrtm = data3;
}

void GalileoNavData::AddOrbit_3(double data0, double data1, double data2, double data3)
{
	this->_orbit.toe = navigation::Seconds{data0};
	this->_orbit.Cic__rad = data1;
	this->_orbit.Omega0__rad = data2;
	this->_orbit.Cis__rad = data3;
}

void GalileoNavData::AddOrbit_4(double data0, double data1, double data2, double data3)
{
	this->_orbit.i0__rad = data0;
	this->_orbit.Crc__m = data1;
	this->_orbit.Omega__rad = data2;
	this->_orbit.Omega_dot__radDs = data3;
}

void GalileoNavData::AddOrbit_5(double data0, double data1, double data2, [[maybe_unused]] double data3)
{
	this->_orbit.Idot__radDs = data0;
	_DataSources = data1;
	_GalWeek = std::chrono::weeks{static_cast<std::chrono::weeks::rep>(data2)};
	_Spare0 = 0.0;

	this->_orbit.toeEpoch = navigation::toeEpoch(_GalWeek, this->_orbit.toe, this->_Epoch.Time());
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
