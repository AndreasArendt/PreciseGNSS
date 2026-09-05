#pragma once

struct PseudorangePrediction
{
    double predicted_m{};
    double residual_m{};
    double variance_m2{};
    Vector3 positionJacobian;
    CorrectionBreakdown corrections;
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