#pragma once

#include "gnss_rtk/core/sv_system.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationType.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationBand.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationAttribute.hpp"

class ObservationDefinition
{
private:
	ObservationType _ObservationType = ObservationType::Unknown;
	ObservationBand _ObservationBand = ObservationBand::Band_Unkown;
	ObservationAttribute _ObservationAttribute = ObservationAttribute::Channel_Unknown;

public:
	// getters
	const ObservationType& GetObservationType() const { return _ObservationType; }
	const ObservationBand& GetObservationBand() const { return _ObservationBand; }
	
	// ctor & dtor
	ObservationDefinition(ObservationType obsType, ObservationBand obsBand, ObservationAttribute obsAttribute) :
		_ObservationType(obsType), _ObservationBand(obsBand), _ObservationAttribute(obsAttribute) { }
	~ObservationDefinition() { }
};

