#pragma once

#include <chrono>

#include "navigation/time.hpp"

class Epoch
{
private:
	navigation::GnssTime _time;

public:
	// getter
	navigation::GnssTime Time() const
	{
		return this->_time;
	}

	// ctor
	Epoch() : _time{} {}
	Epoch(int year, int month, int day, int hour, int minute, double second);
	explicit Epoch(navigation::GnssTime time) : _time(time) {}

	// operator overloading
	bool operator==(const Epoch &other) const
	{
		return std::chrono::abs(this->_time - other.Time()) < navigation::Microseconds{1};
	}

	bool operator!=(const Epoch &other) const
	{
		return !(*this == other);
	}

};
