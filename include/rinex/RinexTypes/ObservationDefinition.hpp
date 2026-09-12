#pragma once

#include "core/sv_system.hpp"
#include "rinex/RinexTypes/ObservationType.hpp"
#include "rinex/RinexTypes/ObservationBand.hpp"
#include "rinex/RinexTypes/ObservationAttribute.hpp"

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
	
	const ObservationAttribute& GetObservationAttribute() const { return _ObservationAttribute; }

	// ctor & dtor
	ObservationDefinition(ObservationType obsType, ObservationBand obsBand, ObservationAttribute obsAttribute) :
		_ObservationType(obsType), _ObservationBand(obsBand), _ObservationAttribute(obsAttribute) { }
	~ObservationDefinition() { }
};

