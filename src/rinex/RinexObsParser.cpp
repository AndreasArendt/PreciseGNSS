//
//  RinexParser.cpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#include "rinex/RinexObsParser/RinexObsParser.hpp"
#include "rinex/RinexTypes/ObservationType.hpp"
#include "rinex/RinexTypes/ObservationBand.hpp"
#include "rinex/RinexTypes/ObservationAttribute.hpp"
#include "rinex/Observations.hpp"
#include "rinex/detail/string_utils.hpp"
#include "rinex/detail/line_reader.hpp"

#include <stdexcept>
#include <utility>
#include <iostream>

const std::string RINEX_VERSION_DEFINITION = "RINEX VERSION / TYPE";
const std::string RINEX_APPROX_POSITION_DEFINITION = "APPROX POSITION XYZ";
const std::string RINEX_ANTENNA_DELTA_DEFINITION  ="ANTENNA: DELTA H/E/N";
const std::string RINEX_OBS_TYPE_DEFINITION = "SYS / # / OBS TYPES";

void RinexObsParser::Parse(std::istream& input)
{
    this->InitParser();
    gnss_rtk::rinex::detail::for_each_line(input, [this](std::string line) {
        this->ParseLine(std::move(line));
    });
}

void RinexObsParser::ReadEpochHeader(std::string line)
{
    // Check if input line has the expected length
    if (line.length() < 33)
    {
        std::cerr << "Error: Invalid epoch header line: " << line << std::endl;
        return;
    }

    try
    {
        // Parse substrings to integers/double
        int year = util::astring::parseInt(line.substr(2, 4));
        int month = util::astring::parseInt(line.substr(7, 2));
        int day = util::astring::parseInt(line.substr(10, 2));
        int hour = util::astring::parseInt(line.substr(13, 2));
        int minute = util::astring::parseInt(line.substr(16, 2));
        double second = util::astring::parseDouble(line.substr(19, 10));
        int epochFlag = util::astring::parseInt(line.substr(31, 1)); // 0: OK; 1: power failure between current and previous epoch; >1 Special Event
        // int numberSVs = parseInt(line.substr(33, 2));

        // Create new Epoch object and add it to the _Epochs vector
        Epoch epoch = Epoch(year, month, day, hour, minute, second);

        this->_Epochs.emplace_back(ObservationEpoch {.time = epoch, .flag = epochFlag, .satellites{} });
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Error: Invalid epoch header line: " << line << std::endl;
    }
}

void RinexObsParser::ReadEpochObservation(std::string line)
{
    auto satellite = SatelliteId(line.substr(0, 3));

    // Filter active
    switch (satellite.SVSystem())
    {
    case SvSystem::GALILEO:
    case SvSystem::GPS:
        break;
    default:
        return;
    }

    this->_Epochs.back().satellites.emplace_back(SatelliteObservation{
        .satellite = satellite,
        .CodeObservations = {},
        .PhaseObservations = {},
        .DopplerObservations = {},
        .SnrObservations = {},
    });

    // See 6.7 in rinex standard how observations are formatted
    unsigned int StartIndex = 3; // offset of first observation
    for (const auto &obsDef : _ObservationDefinitions.at(satellite.SVSystem()))
    {
        // less data avaibale in observation as in specified Header
        if (line.length() < StartIndex + 14)
        {
            continue;
        }

        std::string data = line.substr(StartIndex, 14);

        // no valid data
        if (data.find_first_not_of(' ') == std::string::npos)
        {
            StartIndex += 16;
            continue;
        }

        const auto indicator = [&](std::size_t offset, unsigned int maximum)
            -> std::optional<unsigned int> {
            if (offset >= line.size() || line[offset] == ' ' || line[offset] == '\r')
                return std::nullopt;
            if (line[offset] < '0' || line[offset] > static_cast<char>('0' + maximum))
                throw std::runtime_error("Invalid RINEX quality indicator");
            return static_cast<unsigned int>(line[offset] - '0');
        };
        const Rinex::Observation::QualityIndicators quality{
            .lli = indicator(StartIndex + 14, 7),
            .ssi = indicator(StartIndex + 15, 9)};
        const SignalId signal{obsDef.GetObservationBand(), obsDef.GetObservationAttribute()};

        switch (obsDef.GetObservationType())
        {
        case ObservationType::Code: // Pseudorange
        {
            double psuedorange = util::astring::parseDouble(data);
            this->_Epochs.back().satellites.back().CodeObservations.emplace(signal, Rinex::Observation::Code{psuedorange, quality});

            break;
        }
        case ObservationType::Phase: // Carrierphase
        {
            double cycles = util::astring::parseDouble(data);
            this->_Epochs.back().satellites.back().PhaseObservations.emplace(signal, Rinex::Observation::CarrierPhase{cycles, quality});

            break;
        }
        case ObservationType::Doppler:
        {
            double doppler = util::astring::parseDouble(data);
            this->_Epochs.back().satellites.back().DopplerObservations.emplace(signal, Rinex::Observation::Doppler{doppler, quality});

            break;
        }
        case ObservationType::RawSignalStrength:
        {
            double snr = util::astring::parseDouble(data);
            this->_Epochs.back().satellites.back().SnrObservations.emplace(signal, Rinex::Observation::SignalStrength{snr, quality});

            break;
        }
        case ObservationType::ReceiverChannelNumber:
        {
            break;
        }
        default:
            break;
        }

        StartIndex += 16;
    }
}

void RinexObsParser::ReadObservationTypes(std::string line)
{
    // SYS / # / OBS TYPES definition is: A1 2X,I3 13(1X,A3) [SvSystem (optional)][number of ObsTypes][Type,Band,Attribute]
    SvSystem svSystem = SvSystem::UNKNOWN; // keep svSystem in case is empty!
    if (!std::isspace(line[0]))
    {
        svSystem = static_cast<SvSystem>(line[0]);
    }

    std::string obsDef = "";

    for (auto it = std::begin(line) + 7; it != std::end(line) && !std::isspace(*it); it += 4)
    {
        // Extract the observation definition from the input line
        std::string obsDef(it, it + 3);

        // Convert the observation type, band and attribute
        auto obsType = static_cast<ObservationType>(obsDef[0]);
        auto obsBand = static_cast<ObservationBand>(obsDef[1]);
        auto obsAttribute = static_cast<ObservationAttribute>(obsDef[2]);

        // Add a new ObservationDefinition object
        _ObservationDefinitions[svSystem].emplace_back(obsType, obsBand, obsAttribute);
    }
}

void RinexObsParser::ParseLine(std::string line)
{
    if (line.empty())
        return;

    switch (_RinexParserState)
    {
    case RinexParserState::PARSE_HEADER:
    {
        // new Epoch Found
        if (line[0] == '>')
        {
            _RinexParserState = RinexParserState::PARSE_EPOCH;
            this->ReadEpochHeader(line);
        }
        else if ((line.find(RINEX_VERSION_DEFINITION) != std::string::npos)) // read rinex file version
        {
            std::string str = line.substr(0, 9);
            this->_Version = ltrim(str);
        }
        else if ((line.find(RINEX_APPROX_POSITION_DEFINITION) != std::string::npos)) // approximate position
        {
            double x = util::astring::parseDouble(line.substr(0, 14));
            double y = util::astring::parseDouble(line.substr(14, 14));
            double z = util::astring::parseDouble(line.substr(28, 14));

            this->_ApproximateMarkerPosition = ECEF_Position(x, y, z);
        }
        else if ((line.find(RINEX_ANTENNA_DELTA_DEFINITION) != std::string::npos)) // antenna phase center offset
        {
            double x = util::astring::parseDouble(line.substr(0, 14));
            double y = util::astring::parseDouble(line.substr(14, 14));
            double z = util::astring::parseDouble(line.substr(28, 14));

            this->_AntennaOffset = Position(x, y, z);
        }
        else if (line.find("SIGNAL STRENGTH UNIT") != std::string::npos)
        {
            const std::string unit = line.substr(0, 20);
            const auto first = unit.find_first_not_of(' ');
            _SignalStrengthUnit = first == std::string::npos
                ? std::nullopt
                : std::optional<std::string>{unit.substr(first, unit.find_last_not_of(' ') - first + 1)};
        }
        else if ((line.find(RINEX_OBS_TYPE_DEFINITION) != std::string::npos)) // read all observation types from rnx header
        {
            _RinexParserState = RinexParserState::PARSE_OBS_TYPES;
            this->ReadObservationTypes(line);
        }

        break;
    }
    case RinexParserState::PARSE_OBS_TYPES:
    {
        if ((line.find(RINEX_OBS_TYPE_DEFINITION) != std::string::npos)) // read all observation types from rnx header
        {
            this->ReadObservationTypes(line);
        }
        else
        {
            _RinexParserState = RinexParserState::PARSE_HEADER;
            this->ParseLine(std::move(line));
        }
        break;
    }
    case RinexParserState::PARSE_EPOCH:
    {
        // new Epoch Found
        if (line[0] == '>')
        {
            this->ReadEpochHeader(line);

            // Check if current Epoch is Special Event
            if(this->_Epochs.back().flag > 1)
            {
                _RinexParserState = RinexParserState::PARSE_HEADER; // TODO AA: currently no handling for special events!
            }
        }
        else
        {
            this->ReadEpochObservation(line);
        }

        break;
    }
    default:
        break;
    }
}

void RinexObsParser::InitParser()
{
    this->_RinexParserState = RinexParserState::PARSE_HEADER;
    _SignalStrengthUnit.reset();
    _ObservationDefinitions.clear();
}
