#pragma once

#include "gnss_rtk/core/SatelliteNavigation.hpp"
#include "gnss_rtk/rinex/RinexTypes/IonosphericCorrection.hpp"
#include "gnss_rtk/rinex/RinexTypes/TimeSystemCorrection.hpp"
#include "gnss_rtk/rinex/RinexNavParser/NavEpochParsingState.hpp"

#include <string>
#include <vector>
#include <optional>
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
	std::optional<NavigationMessage> _CurrentNavData;
	std::vector<SatelliteNavigation>& _Satellites;
	SatelliteNavigation* _CurrentSatellite = nullptr;

	SatelliteNavigation* CurrentSatellite() const { return _CurrentSatellite; }
	void FindCurrentSatellite(SatelliteId satellite);
	void StoreCurrentNavData();

	void ParseEpoch(std::string line);
	void ParseLine(std::string line);
	void ParseIonoCorrDefinition(std::string line);
	void ParseTimeDiffDefinition(std::string line);
	void ParseOrbitData(std::string line);

	void InitParser();
public:
	// getters
	std::string const& Version() const { return this->_Version; }
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }
	std::vector<TimeSystemCorrection> const& TimeSystemCorrections() const { return this->_TimeSystemCorrections; }

	// ctor & dtor
	explicit RinexNavParser(std::vector<SatelliteNavigation>& satellites) : _Satellites(satellites) {}
	~RinexNavParser() = default;

	// public function
	void Parse(std::istream& input);
};
