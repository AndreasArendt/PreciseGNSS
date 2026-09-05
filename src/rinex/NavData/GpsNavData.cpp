#include "rinex/NavData/Gps/GpsNavData.hpp"

void GpsNavData::AddClockErrors(double data0, double data1, double data2)
{
	this->_clock = navigation::PolynomialClockData{
		.bias = navigation::Seconds{data0},
		.drift = navigation::ClockDrift{data1},
		.driftRate = navigation::ClockDriftRate{data2},
		.referenceTime = _Epoch.Time()};
}

void GpsNavData::AddOrbit_1(double data0, double data1, double data2, double data3)
{
	_IODE_IssueOfData = data0;
	this->_orbit.Crs__m = data1;
	this->_orbit.DeltaN__radDs = data2;
	this->_orbit.M0__rad = data3;
}

void GpsNavData::AddOrbit_2(double data0, double data1, double data2, double data3)
{
	this->_orbit.Cuc__rad = data0;
	this->_orbit.Eccentricity = data1;
	this->_orbit.Cus__rad = data2;
	this->_orbit.SqrtA___sqrtm = data3;
}

void GpsNavData::AddOrbit_3(double data0, double data1, double data2, double data3)
{	
	this->_orbit.toe = navigation::Seconds{data0};
	this->_orbit.Cic__rad = data1;
	this->_orbit.Omega0__rad = data2;
	this->_orbit.Cis__rad = data3;
}

void GpsNavData::AddOrbit_4(double data0, double data1, double data2, double data3)
{
	this->_orbit.i0__rad = data0;
	this->_orbit.Crc__m = data1;
	this->_orbit.Omega__rad = data2;
	this->_orbit.Omega_dot__radDs = data3;
}

void GpsNavData::AddOrbit_5(double data0, double data1, double data2, double data3)
{
	this->_orbit.Idot__radDs = data0;
	_CodesOnL2 = data1;
	_GpsWeek = std::chrono::weeks{static_cast<std::chrono::weeks::rep>(data2)};
	_L2P_DataFlag = data3;

	this->_orbit.toeEpoch = navigation::toeEpoch(_GpsWeek, this->_orbit.toe, this->_Epoch.Time());
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
