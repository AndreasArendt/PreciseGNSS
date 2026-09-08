#pragma once

#include <array>

#include "navigation/ephemeris.hpp"
#include "positioning/correction_context.hpp"
#include "positioning/measurements/code_observation.hpp"
#include "positioning/receiver_state.hpp"

struct PseudorangePrediction
{
    double predicted_m{};
    // Derivative of predicted_m with respect to receiver ECEF x, y, z.
    std::array<double, 3> d_predicted_d_receiver_position{};
    double d_predicted_d_receiver_clock_bias{};
    CorrectionBreakdown corrections{};
};

class PseudorangeModel
{
public:
    PseudorangePrediction Evaluate(
        const CodeObservation &observation,
        const ReceiverState &receiver,
        const SatelliteState &satellite,
        const CorrectionContext &context) const;
};
