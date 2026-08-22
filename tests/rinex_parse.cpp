#include <cstdlib>
#include <iostream>

#include "gnss_rtk/rinex/RinexParser.hpp"

int main() {
    
    auto rnx_parse = RinexParser();
    rnx_parse.Parse("/workspaces/PreciseGnss/data/FLY_0204.obs");

    return EXIT_SUCCESS;
}
