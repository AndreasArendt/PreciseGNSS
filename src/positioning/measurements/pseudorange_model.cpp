#include <cmath>
#include <stdexcept>
#include <vector>

#include "coordinates/transformation.hpp"
#include "positioning/measurements/pseudorange_model.hpp"

// Pseudorange model: p^s_r + c(\delta t_r - \delta t^s) + T + I + e

PseudorangePrediction PseudorangeModel::Evaluate(
    const CodeObservation &observation, const ReceiverState &receiver,
    const SatelliteState &satellite, const CorrectionContext &context) const {

  Position delta_pos = satellite.Position_E - receiver.position;
  double geometric_distance__m =
      std::hypot(delta_pos.x(), delta_pos.y(), delta_pos.z());

  if (!std::isfinite(geometric_distance__m) || geometric_distance__m <= 0.0)
    throw std::invalid_argument(
        "Distance between receiver and satellite is 0 or nonfinite");

  // todo use proper cross product function later! ([0;0;w] x [x;y;z]^s)
  std::vector<double> cross_w_x_xsv{
      -Transformation::MeanAngularVelocityOfEarth__radDs *
          satellite.Position_E.y(),
      Transformation::MeanAngularVelocityOfEarth__radDs *
          satellite.Position_E.x(),
      0};

  double sagnac__m =
      -(delta_pos.x() * cross_w_x_xsv[0] + delta_pos.y() * cross_w_x_xsv[1] +
        delta_pos.z() * cross_w_x_xsv[2]) /
      Transformation::SpeedOfLight__mDs;

  navigation::Seconds sv_clock_bias =
      satellite.clock.bias + satellite.relativisticClockBias;

  double sv_clock_error__m =
      Transformation::SpeedOfLight__mDs * sv_clock_bias.count();

  double predicted__m = geometric_distance__m +
                        (receiver.clockBias_m - sv_clock_error__m) + sagnac__m +
                        context.troposphere_m + context.ionosphere_m +
                        context.groupDelay_m;

  return PseudorangePrediction{
      .predicted_m = predicted__m,
      .d_predicted_d_receiver_position{
          -delta_pos.x() / geometric_distance__m +
              cross_w_x_xsv[0] / Transformation::SpeedOfLight__mDs,

          -delta_pos.y() / geometric_distance__m +
              cross_w_x_xsv[1] / Transformation::SpeedOfLight__mDs,

          -delta_pos.z() / geometric_distance__m,
      },
      .d_predicted_d_receiver_clock_bias = 1.0,
      .corrections{.satelliteClock_m{-sv_clock_error__m},
                   .earthRotation_m{sagnac__m},
                   .troposphere_m{context.troposphere_m},
                   .ionosphere_m{context.ionosphere_m},
                   .groupDelay_m{context.groupDelay_m}}};
}
