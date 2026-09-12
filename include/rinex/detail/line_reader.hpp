#pragma once

#include <istream>
#include <string>
#include <utility>

namespace constants::rinex::detail {

template <typename LineHandler>
void for_each_line(std::istream& input, LineHandler&& handle_line) {
    std::string line;
    while (std::getline(input, line)) {
        std::forward<LineHandler>(handle_line)(line);
    }
}

}  // namespace constants::rinex::detail
