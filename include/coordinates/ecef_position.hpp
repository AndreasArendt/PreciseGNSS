#pragma once

#include "coordinates/position.hpp"

class ECEF_Position : public Position
{
private:	
public:
	// ctor & dtor
	ECEF_Position() : Position() {}
	ECEF_Position(double x, double y, double z) : Position(x,y,z) {}
	explicit ECEF_Position(const Eigen::Vector3d &coordinates_m) : Position(coordinates_m) {}
	~ECEF_Position() {}
};
