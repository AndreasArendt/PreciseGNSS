#pragma once

class GpsSvHealth
{
private:
    int _Health;

public: 
    int Health() const { return this->_Health; }

    static GpsSvHealth fromBitfield(double svHealth)
    {
        GpsSvHealth result;
        result._Health = (int)svHealth;
        return result;
    }
};