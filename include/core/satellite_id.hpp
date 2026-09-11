#pragma once

#include <format>
#include <string>

#include "core/sv_system.hpp"
#include "rinex/detail/string_utils.hpp"

class SatelliteId {
private:
  SvSystem _SvSystem;
  int _SvNumber;

public:
  SvSystem const &SVSystem() const { return this->_SvSystem; }
  int const &SvNumber() const { return this->_SvNumber; }

  std::string toString() const {
    return std::format("{}{:02}", static_cast<char>(_SvSystem), _SvNumber);
  }

  SatelliteId() : _SvSystem(SvSystem::UNKNOWN), _SvNumber(-1) {}
  SatelliteId(SvSystem svSystem, int svNumber)
      : _SvSystem(svSystem), _SvNumber(svNumber) {}
  explicit SatelliteId(std::string satellite)
      : _SvSystem(static_cast<SvSystem>(satellite.at(0))),
        _SvNumber(util::astring::parseInt(satellite.substr(1, 2))) {}

  bool operator==(const SatelliteId &other) const = default;
};
