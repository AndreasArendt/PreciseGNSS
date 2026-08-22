//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "gnss_rtk/rinex/RinexTypes/Satellite.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationDefinition.hpp"
#include "gnss_rtk/rinex/RinexObsParser/RinexReaderState.hpp"
#include "gnss_rtk/coordinates/position.hpp"
#include "gnss_rtk/core/epoch.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <istream>

class RinexObsParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);    
    
    std::string _Version;    
    std::unordered_map<SvSystem, std::vector<ObservationDefinition>> _ObservationDefinitions;
    RinexParserState _RinexParserState = RinexParserState::PARSE_HEADER;
    ECEF_Position _ApproximateMarkerPosition;
    Position _AntennaOffset;
    Epoch _CurrentEpoch;
    ObsData _CurrentObsData;
    int _CurrentEpochFlag;
    std::vector<Satellite>& _Satellites;
    Satellite* _CurrentSatellite = nullptr;

    Satellite* CurrentSatellite() const { return _CurrentSatellite; }
    void FindCurrentSatellite(Satellite satellite);
    
public:
    //getters    
    std::string const& Version() const { return _Version; }
    ECEF_Position const& ApproximateMarkerPosition() const { return _ApproximateMarkerPosition; }
    Position const& AntennaOffset() const { return _AntennaOffset; }
        
    // ctor & dtor
    explicit RinexObsParser(std::vector<Satellite>& satellites)
        : _CurrentEpochFlag(-1), _Satellites(satellites) {}
    ~RinexObsParser();
        
    //functions
    void InitParser();
    void Parse(std::istream& input);
};
