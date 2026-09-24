#pragma once

#include <stdexcept>

#include <gtsam/geometry/Point3.h>
#include <gtsam/nonlinear/NonlinearFactor.h>

#include "positioning/measurements/pseudorange_model.hpp"

// One code observation connects receiver position and clock bias (metres).
// Satellite state and precomputed corrections are fixed inputs, not graph
// nodes.
class PseudorangeFactor
    : public gtsam::NoiseModelFactor2<gtsam::Point3, double> {

private:
  CodeObservation _observation;
  SatelliteState _satellite;
  CorrectionContext _corrections;
  PseudorangeModel _model;

public:
  using Base = gtsam::NoiseModelFactor2<gtsam::Point3, double>;

  PseudorangeFactor(gtsam::Key positionKey, gtsam::Key clockKey,
                    const CodeObservation &observation,
                    const SatelliteState &satellite,
                    const CorrectionContext &corrections,
                    const gtsam::SharedNoiseModel &noise)
      : Base(noise, positionKey, clockKey), _observation(observation),
        _satellite(satellite), _corrections(corrections) {
    if (!noise || noise->dim() != 1) {
      throw std::invalid_argument(
          "PseudorangeFactor requires a one-dimensional noise model");
    }
  }

  gtsam::Vector evaluateError(
      const gtsam::Point3 &position, const double &clockBias_m,
      boost::optional<gtsam::Matrix &> Hposition = boost::none,
      boost::optional<gtsam::Matrix &> Hclock = boost::none) const override {
    ReceiverState receiver;
    receiver.position = ECEF_Position{position.x(), position.y(), position.z()};
    receiver.clockBias_m = clockBias_m;

    const auto prediction =
        _model.Evaluate(_observation, receiver, _satellite, _corrections);

    // Jacobians of h(x) - observed, before noise-model whitening.
    if (Hposition) {
      Hposition->resize(1, 3);
      for (Eigen::Index i = 0; i < 3; ++i) {
        (*Hposition)(0, i) =
            prediction.d_predicted_d_receiver_position[static_cast<size_t>(i)];
      }
    }

    if (Hclock) {
      Hclock->resize(1, 1);
      (*Hclock)(0, 0) = prediction.d_predicted_d_receiver_clock_bias;
    }

    return gtsam::Vector1(prediction.predicted_m -
                          _observation.code.pseudorange_m);
  }
};
