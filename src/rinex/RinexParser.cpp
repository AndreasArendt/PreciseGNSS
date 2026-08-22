#include "gnss_rtk/rinex/RinexParser.hpp"
#include "gnss_rtk/rinex/RinexNavParser/RinexNavParser.hpp"
#include "gnss_rtk/rinex/RinexObsParser/RinexObsParser.hpp"

#include <fstream>
#include <stdexcept>

void RinexParser::Parse(std::string path)
{
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Could not open RINEX file: " + path);
    }

    std::string first_line;
    std::getline(input, first_line);
    input.clear();
    input.seekg(0);

    if ((first_line.find("NAVIGATION DATA") != std::string::npos) ||
        (first_line.find("NAV DATA") != std::string::npos)) {
        RinexNavParser parser(this->_Satellites);
        parser.Parse(input);
    } else if (first_line.find("OBSERVATION DATA") != std::string::npos) {
        RinexObsParser parser(this->_Satellites);
        parser.Parse(input);
    } else {
        throw std::runtime_error("Unknown RINEX file type: " + path);
    }
}
