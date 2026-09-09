#pragma once

#include <vector>
#include <optional>

#include "positioning/positioning_epoch.hpp"
#include "positioning/receiver_state.hpp"

struct SPPState {
    std::optional<ReceiverState> receiverState;
    navigation::GnssTime epochtime;
};

class SPPSolver {

private:
public:
    std::vector<SPPState> Solve(const std::vector<PositioningEpoch>& epochs);
};