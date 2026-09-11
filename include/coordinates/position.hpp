#pragma once

#include <cmath>

class Position {
private:
  double _x__m = 0;
  double _y__m = 0;
  double _z__m = 0;

public:
  // getters
  double x() const { return _x__m; }
  double y() const { return _y__m; }
  double z() const { return _z__m; }

  double norm() const { return std::hypot(_x__m, _y__m, _z__m); }

  // ctor & dtor
  Position() {}
  Position(double x, double y, double z) : _x__m(x), _y__m(y), _z__m(z) {}
  ~Position() {}

  Position operator+(const Position &rhs) const {
    return Position(this->_x__m + rhs._x__m, this->_y__m + rhs._y__m,
                    this->_z__m + rhs._z__m);
  }

  Position operator-(const Position &rhs) const {
    return Position(this->_x__m - rhs._x__m, this->_y__m - rhs._y__m,
                    this->_z__m - rhs._z__m);
  }
};