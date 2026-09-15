#include <cmath>
#include <numbers>
#include <stdexcept>

#include "core/constants.hpp"
#include "positioning/corrections/troposphere/saastamoinen.hpp"

namespace {

// BERG model
double standard_atmosphere(double h) {
  return 1013.25 * std::pow(1.0 - 2.25e-5 * h, 5.225);
}

double mapping_function__Mh_Chao(double elevation__rad) {
  return 1 / (std::sin(elevation__rad) +
              (0.00143 / (std::tan(elevation__rad) + 0.0445)));
}

double mapping_function__Mw_Chao(double elevation__rad) {
  return 1 / (std::sin(elevation__rad) +
              (0.00035 / (std::tan(elevation__rad) + 0.017)));
}
} // namespace

troposphere::Delay troposphere::SaastamoinenChao(const Input &input) {
  constexpr double half_pi = std::numbers::pi / 2.0;
  if (!std::isfinite(input.latitude_rad) ||
      std::abs(input.latitude_rad) > half_pi ||
      !std::isfinite(input.height_m) || input.height_m < -100.0 ||
      input.height_m > 10000.0 || !std::isfinite(input.elevation_rad) ||
      input.elevation_rad <= 0.0 || input.elevation_rad > half_pi) {
    throw std::invalid_argument(
        "SaastamoinenChao: input outside supported domain");
  }

  // gravity at equator - compensated by latitude
  double g_eff = 9.7840 * (1.0 - 0.00266 * std::cos(2.0 * input.latitude_rad) -
                           0.28e-6 * input.height_m);

  double k1 = 77.6890; // +/- 0.015;
  double p0 = standard_atmosphere(input.height_m);

  // hydrostatic delay
  double zhd_m = 1e-6 * k1 *
                 ((constants::UniversalGasConstant * p0) /
                  (constants::MeanMolarMass_DryAir * g_eff));

  // Approximate temperature when measurements are unavailable.
  double Ts = 288.15 - 0.0065 * input.height_m;

  double relative_humidity = 0.70; // Assumed fraction, not percent
  double saturation_pressure_hPa =
      6.108 * std::exp((17.15 * Ts - 4684.0) / (Ts - 38.45));

  double pws = relative_humidity * saturation_pressure_hPa;
  double zwd_m = 0.0022768 * (1255.0 / Ts + 0.05) * pws;

  return {.hydrostatic_m =
              mapping_function__Mh_Chao(input.elevation_rad) * zhd_m,
          .wet_m = mapping_function__Mw_Chao(input.elevation_rad) * zwd_m};
}
