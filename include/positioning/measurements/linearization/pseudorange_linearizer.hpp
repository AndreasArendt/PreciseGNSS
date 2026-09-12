#pragma once

#include "positioning/measurements/linearization/linearized_system.hpp"
#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"
#include "positioning/correction_context.hpp"

bool LinearizeEpoch(const PositioningEpoch &epoch,
                    const ReceiverState &receiver,
                    const CorrectionContext &context,
                    LinearizedSystem &linearizedSystem);