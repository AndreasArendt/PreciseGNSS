#pragma once

#include "coordinates/ecef_position.hpp"

struct ReceiverState
{
    ECEF_Position position;
    // Receiver clock offset multiplied by the speed of light, in metres.
    double clockBias_m{};
};
