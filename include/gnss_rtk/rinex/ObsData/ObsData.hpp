#pragma once

#include "gnss_rtk/rinex/Observations/CodeObservation.hpp"
#include "gnss_rtk/rinex/Observations/PhaseObservation.hpp"
#include "gnss_rtk/rinex/Observations/DopplerObservation.hpp"
#include "gnss_rtk/rinex/Observations/SignalStrengthObservation.hpp"
#include "gnss_rtk/rinex/RinexTypes/ObservationBand.hpp"
#include "gnss_rtk/rinex/RinexData.hpp"
#include "gnss_rtk/core/epoch.hpp"

#include <map>

class ObsData : public RinexData
{
private:
    int _EpochFlag;    
    bool _IsSpecialEvent;

    std::map<ObservationBand, CodeObservation> _CodeObservations;
    std::map<ObservationBand, PhaseObservation> _PhaseObservations;
    std::map<ObservationBand, DopplerObservation> _DopplerObservations;
    std::map<ObservationBand, SignalStrengthObservation> _SnrObservations;

public:
    // getters            
    std::map<ObservationBand, CodeObservation> const& CodeObservations() const { return this->_CodeObservations; }
    std::map<ObservationBand, PhaseObservation> const& PhaseObservations() const { return this->_PhaseObservations; }
    std::map<ObservationBand, DopplerObservation> const& DopplerObservations() const { return this->_DopplerObservations; }
    std::map<ObservationBand, SignalStrengthObservation> const& SnrObservations() const { return this->_SnrObservations; }
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

