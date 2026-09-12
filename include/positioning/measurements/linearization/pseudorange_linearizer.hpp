#pragma once

#include <optional>

#include "positioning/measurements/linearization/linearized_system.hpp"
#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"
#include "positioning/correction_context.hpp"

std::optional<LinearizedSystem> LinearizePseudorangeEpoch(const PositioningEpoch &epoch,
                               const ReceiverState &receiver,
                               const CorrectionContext &context);