#include "core/satellite_id.hpp"

#include "rinex/detail/string_utils.hpp"

SatelliteId::SatelliteId() : _SvSystem(SvSystem::UNKNOWN), _SvNumber(-1)
{
}

SatelliteId::SatelliteId(SvSystem svSystem, int svNumber)
    : _SvSystem(svSystem), _SvNumber(svNumber)
{
}

SatelliteId::SatelliteId(std::string satellite)
    : _SvSystem(static_cast<SvSystem>(satellite.at(0))),
      _SvNumber(util::astring::parseInt(satellite.substr(1, 2)))
{
}
