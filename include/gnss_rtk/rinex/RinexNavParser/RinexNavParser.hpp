#pragma once

#include "gnss_rtk/rinex/RinexTypes/Satellite.hpp"
#include "gnss_rtk/rinex/RinexTypes/IonosphericCorrection.hpp"
#include "gnss_rtk/rinex/RinexTypes/TimeSystemCorrection.hpp"
#include "gnss_rtk/rinex/RinexTypes/Satellite.hpp"
#include "gnss_rtk/rinex/RinexNavParser/NavEpochParsingState.hpp"

#include <string>
#include <vector>
#include <memory>
#include <istream>

class RinexNavParser
{
private:
	std::string _Version;
	std::string _Type;
	std::vector<IonosphericCorrection> _IonosphericCorrections;
	std::vector<TimeSystemCorrection> _TimeSystemCorrections;	
			
	bool _RinexHeaderParsed = false;
	NavEpochParsingState _NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;

	ENavOrbitNumber _CurrentOrbitNumber = ENavOrbitNumber::ORBIT_UNKNOWN;	
	std::unique_ptr<NavData> _CurrentNavData;
	std::vector<Satellite>& _Satellites;
	Satellite* _CurrentSatellite = nullptr;

	Satellite* CurrentSatellite() const { return _CurrentSatellite; }
	void FindCurrentSatellite(Satellite satellite);
	
	void ParseEpoch(std::string line);	
	void ParseLine(std::string line);
	void ParseIonoCorrDefinition(std::string line);
	void ParseTimeDiffDefinition(std::string line);
	void ParseOrbitData(std::string line);
	
public:
	// getters
	std::string const& Version() const { return this->_Version; }	
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }
	std::vector<TimeSystemCorrection> const& TimeSystemCorrections() const { return this->_TimeSystemCorrections; }
	
	// ctor & dtor	
	explicit RinexNavParser(std::vector<Satellite>& satellites) : _Satellites(satellites) {}
	~RinexNavParser();

	// public function
	void InitParser();
	void Parse(std::istream& input);
};

