#pragma once

#include "positioning/corrections/troposphere.hpp"

namespace troposphere {

// Saastamoinen zenith delays with separate Chao hydrostatic/wet mappings.
// Uses a standard atmosphere and fixed 70% relative humidity.
// Prototype domain: height [-100, 10000] m above mean sea level,
// latitude [-pi/2, pi/2], elevation (0, pi/2], all inputs finite.
// Returns std::nullopt outside this domain. The caller applies its
// observation elevation mask; positive elevation alone does not assure accuracy.
std::optional<Delay> SaastamoinenChao(const Input &input);

} // namespace troposphere
