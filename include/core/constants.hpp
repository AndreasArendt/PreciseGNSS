#pragma once

namespace constants {

inline constexpr double GravitationalConstant__m3Ds2 = 3.986004418e14;
inline constexpr double MeanAngularVelocityOfEarth__radDs = 7.2921151467e-5;
inline constexpr double SpeedOfLight__mDs = 299792458.0;

inline constexpr double WGS84SemiMajorAxis__m = 6378137.0;
inline constexpr double WGS84Flattening = 1.0 / 298.257223563;
inline constexpr double WGS84SemiMinorAxis__m =
    WGS84SemiMajorAxis__m * (1.0 - WGS84Flattening);
inline constexpr double kWgs84FirstEccentricitySquared =
    WGS84Flattening * (2.0 - WGS84Flattening);

} // namespace constants
