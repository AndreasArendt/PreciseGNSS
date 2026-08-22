#pragma once

#include "gnss_rtk/rinex/RinexTypes/Satellite.hpp"

#include <vector>
#include <string>

class RinexParser
{
private:
	std::vector<Satellite> _Satellites;

public:			
	// getters
	std::vector<Satellite> const& Satellites() const { return this->_Satellites; }

	// public functions
	void Parse(std::string path);

	//ctor & dtor
	~RinexParser() = default;
	RinexParser() = default;
};

