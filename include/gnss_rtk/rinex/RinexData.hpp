#pragma once

#include "gnss_rtk/core/epoch.hpp"

class RinexData
{
protected:
	Epoch _Epoch;

public:
	RinexData() = default;
	RinexData(Epoch epoch) : _Epoch(epoch) {}
	~RinexData() = default;

	Epoch const& Epoche() const { return this->_Epoch; }
};
