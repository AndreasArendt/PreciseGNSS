#include "positioning/corrections/troposphere/saastamoinen.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numbers>
#include <stdexcept>

int main() {
  // Exercise selection through the common interface, without solver context.
  const troposphere::Model model = &troposphere::SaastamoinenChao;
  constexpr double pi = std::numbers::pi;
  const auto zenith = model({pi / 4.0, 0.0, pi / 2.0});
  if (std::abs(zenith.hydrostatic_m - 2.3094967381) > 1e-8 ||
      std::abs(zenith.wet_m - 0.1204034929) > 1e-8 ||
      std::abs(zenith.Total() - 2.4299002310) > 1e-8) {
    std::cerr << "Unexpected sea-level zenith delay\n";
    return EXIT_FAILURE;
  }

  double previous = zenith.Total();
  for (double degrees : {60.0, 30.0, 10.0, 5.0}) {
    const auto delay = model({pi / 4.0, 0.0, degrees * pi / 180.0});
    if (!std::isfinite(delay.Total()) || delay.Total() <= previous ||
        delay.hydrostatic_m <= zenith.hydrostatic_m ||
        delay.wet_m <= zenith.wet_m) {
      std::cerr << "Delay must increase as elevation decreases\n";
      return EXIT_FAILURE;
    }
    previous = delay.Total();
  }
  const auto low = model({pi / 4.0, 0.0, pi / 6.0});
  if (std::abs(low.Total() - 4.838365) > 1e-5 ||
      low.hydrostatic_m / zenith.hydrostatic_m >=
          low.wet_m / zenith.wet_m) {
    std::cerr << "Unexpected separate hydrostatic/wet mapping\n";
    return EXIT_FAILURE;
  }

  const double nan = std::numeric_limits<double>::quiet_NaN();
  for (const auto input : {
           troposphere::Input{0.0, 0.0, 0.0},
           troposphere::Input{0.0, 0.0, -0.1},
           troposphere::Input{0.0, 0.0, pi},
           troposphere::Input{pi, 0.0, 1.0},
           troposphere::Input{0.0, -101.0, 1.0},
           troposphere::Input{0.0, 10001.0, 1.0},
           troposphere::Input{nan, 0.0, 1.0},
           troposphere::Input{0.0, nan, 1.0},
           troposphere::Input{0.0, 0.0, nan}}) {
    try {
      model(input);
      std::cerr << "Invalid input was accepted\n";
      return EXIT_FAILURE;
    } catch (const std::invalid_argument &) {
    }
  }
  return EXIT_SUCCESS;
}
