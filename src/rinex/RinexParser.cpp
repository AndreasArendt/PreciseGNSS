#include "rinex/RinexParser.hpp"

#include "rinex/RinexNavParser/RinexNavParser.hpp"
#include "rinex/RinexObsParser/RinexObsParser.hpp"

#include <fstream>
#include <stdexcept>

RinexFile RinexParser::Parse(const std::filesystem::path& path) const
{
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Could not open RINEX file: " + path.string());
    }

    std::string firstLine;
    std::getline(input, firstLine);
    input.clear();
    input.seekg(0);

    if ((firstLine.find("NAVIGATION DATA") != std::string::npos) ||
        (firstLine.find("NAV DATA") != std::string::npos)) {
        NavigationFile result;
        RinexNavParser parser(result.satellites);
        parser.Parse(input);

        result.version = parser.Version();
        result.type = parser.Type();
        result.ionosphericCorrections = parser.IonosphericCorrections();
        result.timeSystemCorrections = parser.TimeSystemCorrections();
        return result;
    }

    if (firstLine.find("OBSERVATION DATA") != std::string::npos) {
        ObservationFile result;
        RinexObsParser parser(result.epochs);
        parser.Parse(input);

        result.version = parser.Version();
        result.approximateMarkerPosition = parser.ApproximateMarkerPosition();
        result.antennaOffset = parser.AntennaOffset();
        return result;
    }

    throw std::runtime_error("Unknown RINEX file type: " + path.string());
}
