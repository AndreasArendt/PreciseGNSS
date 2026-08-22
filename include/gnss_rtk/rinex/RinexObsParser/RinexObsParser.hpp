//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "gnss_rtk/rinex/RinexTypes/ObservationDefinition.hpp"
#include "gnss_rtk/rinex/RinexObsParser/RinexReaderState.hpp"
#include "gnss_rtk/coordinates/position.hpp"
#include "gnss_rtk/coordinates/ecef_position.hpp"
#include "gnss_rtk/core/epoch.hpp"
#include "gnss_rtk/core/ObservationEpoch.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <istream>

class RinexObsParser
{

private:
    std::string _Version;
    std::unordered_map<SvSystem, std::vector<ObservationDefinition>> _ObservationDefinitions;
    RinexParserState _RinexParserState = RinexParserState::PARSE_HEADER;
    ECEF_Position _ApproximateMarkerPosition;
    Position _AntennaOffset;
    std::vector<ObservationEpoch> &_Epochs;

    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);

    void InitParser();

public:
    // getters
    std::string const &Version() const { return _Version; }
    ECEF_Position const &ApproximateMarkerPosition() const { return _ApproximateMarkerPosition; }
    Position const &AntennaOffset() const { return _AntennaOffset; }

    // ctor & dtor
    explicit RinexObsParser(std::vector<ObservationEpoch> &epochs) : _Epochs(epochs) {}
    ~RinexObsParser() = default;

    // functions
    void Parse(std::istream &input);
};
