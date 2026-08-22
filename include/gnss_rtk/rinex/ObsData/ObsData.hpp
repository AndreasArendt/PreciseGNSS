#pragma once

#include "gnss_rtk/rinex/Observations.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationBand.hpp"
#include "gnss_rtk/rinex/RinexData.hpp"
#include "gnss_rtk/core/epoch.hpp"

#include <map>

using namespace Rinex::Observation;

class ObsData : public RinexData
{
private:
    int _EpochFlag;    
    bool _IsSpecialEvent;

    std::map<ObservationBand, Code> _CodeObservations;
    std::map<ObservationBand, CarrierPhase> _PhaseObservations;
    std::map<ObservationBand, Doppler> _DopplerObservations;
    std::map<ObservationBand, SignalStrength> _SnrObservations;

public:
    // getters            
    std::map<ObservationBand, Code> const& CodeObservations() const { return this->_CodeObservations; }
    std::map<ObservationBand, CarrierPhase> const& PhaseObservations() const { return this->_PhaseObservations; }
    std::map<ObservationBand, Doppler> const& DopplerObservations() const { return this->_DopplerObservations; }
    std::map<ObservationBand, SignalStrength> const& SnrObservations() const { return this->_SnrObservations; }
    bool const& IsSpecialEvent() const { return this->_IsSpecialEvent; }        
    
    // functions
    void AddCodeObservation(ObservationBand band, double Pseudorange__m);
    void AddPhaseObservation(ObservationBand band, double Carrierphase__Cycles);
    void AddDopplerObservation(ObservationBand band, double Doppler__Hz);
    void AddSnrObservation(ObservationBand band, double SNR);

    ObsData() : _EpochFlag(-1), _IsSpecialEvent(false) { }
    ObsData(Epoch epoch, int epochFlag) : RinexData(epoch), _EpochFlag(epochFlag), _IsSpecialEvent(epochFlag >= 2) { }    
    ~ObsData() { }
};

