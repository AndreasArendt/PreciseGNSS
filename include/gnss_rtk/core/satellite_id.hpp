#pragma once

#include "gnss_rtk/core/sv_system.hpp"

#include <format>
#include <string>

class SatelliteId
{
private:
    SvSystem _SvSystem;
    int _SvNumber;

public:
    SvSystem const& SVSystem() const { return this->_SvSystem; }
    int const& SvNumber() const { return this->_SvNumber; }
    int SvIdentifier() const {
        return (static_cast<int>(static_cast<unsigned char>(this->_SvSystem)) << 8)
            + (this->_SvNumber & 0xFF);
    }
    std::string SvString() const {
        return std::format("{}{}", static_cast<char>(this->_SvSystem), this->_SvNumber);
    }

    SatelliteId();
    SatelliteId(SvSystem svSystem, int svNumber);
    explicit SatelliteId(std::string satellite);

    bool operator==(const SatelliteId& other) const = default;
};
