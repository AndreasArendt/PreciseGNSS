#pragma once

#include "gnss_rtk/core/sv_system.hpp"

#include <string>
#include <format>

class AbstractSatellite
{
protected:
	SvSystem _SvSystem;
	int _SvNumber;

public:
	// getters
	SvSystem const& SVSystem() const { return this->_SvSystem; }
	int const& SvNumber() const { return this->_SvNumber; }
	int SvIdentifier() const { return (static_cast<int>(static_cast<unsigned char>(this->_SvSystem)) << 8) + (this->_SvNumber & 0xFF); }
	std::string SvString() const { return std::format("{}{}", static_cast<char>(this->_SvSystem), this->_SvNumber); }
	
	// ctor & dtor    
	AbstractSatellite();
	AbstractSatellite(SvSystem svSystem, int svNumber);
	AbstractSatellite(std::string satStr);
	AbstractSatellite(const AbstractSatellite& other);
	~AbstractSatellite() = default;

	// Operator overloading
	AbstractSatellite& operator=(AbstractSatellite& other) noexcept;
	bool operator==(const AbstractSatellite& other) const;
	bool operator!=(const AbstractSatellite& other) const;
};
