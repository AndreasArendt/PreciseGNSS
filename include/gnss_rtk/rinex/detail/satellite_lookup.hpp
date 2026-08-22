#pragma once

#include "gnss_rtk/rinex/RinexTypes/Satellite.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace gnss_rtk::rinex::detail {

inline Satellite& find_or_add_satellite(
    std::vector<Satellite>& satellites,
    Satellite satellite) {
    const auto iterator = std::find(satellites.begin(), satellites.end(), satellite);
    if (iterator != satellites.end()) {
        return *iterator;
    }

    satellites.push_back(std::move(satellite));
    return satellites.back();
}

}  // namespace gnss_rtk::rinex::detail
