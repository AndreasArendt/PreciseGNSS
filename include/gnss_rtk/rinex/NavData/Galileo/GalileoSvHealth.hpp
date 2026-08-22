#pragma once

#include "gnss_rtk/rinex/NavData/SvHealth.hpp"

class GalileoSvHealth : public SvHealth
{
public:
    struct
    {
        unsigned int E1B_DVS : 1;
        unsigned int E1B_HS : 2;
        unsigned int E5A_DVS : 1;
        unsigned int E5A_HS : 2;
        unsigned int E5B_DVS : 1;
        unsigned int E5B_HS : 2;
    } bits = {};

    int const &Health() const { return this->_Health; }

    static GalileoSvHealth fromBitfield(double svHealth)
    {
        GalileoSvHealth result;
        result._Health = (int)svHealth;

        result.bits.E1B_DVS = (result._Health >> 0) & 0x1;
        result.bits.E1B_HS = (result._Health >> 1) & 0x3;
        result.bits.E5A_DVS = (result._Health >> 3) & 0x1;
        result.bits.E5A_HS = (result._Health >> 4) & 0x3;
        result.bits.E5B_DVS = (result._Health >> 6) & 0x1;
        result.bits.E5B_HS = (result._Health >> 7) & 0x3;

        return result;
    }

    GalileoSvHealth() : SvHealth() {}
    ~GalileoSvHealth() {}
};