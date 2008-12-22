#include <polygon.h>

#include <cstring>

Polygon::Polygon(size_t num, const int16_t* x, const int16_t* y, struct Colour colour) 
	: _num(num), _colour(colour) {
	_x = new int16_t[_num];
	_y = new int16_t[_num];

	::memcpy(_x, x, _num * sizeof(int16_t));
	::memcpy(_y, y, _num * sizeof(int16_t));
}

Polygon::Polygon(const Polygon& other) 
	: _cap(0), _num(0), _x(NULL), _y(NULL) {
	operator=(other);
}

Polygon::~Polygon() {
}

Polygon& Polygon::operator=(const Polygon& rhs) { 
	if (_cap < rhs.num()) {
		if (_cap) {
			delete[] _x;
			delete[] _y;
		}
		_x = new int16_t[rhs.num()];
		_y = new int16_t[rhs.num()];
		_cap = rhs.num();
	}

	_num = rhs.num();
	::memcpy(_x, rhs.x(), _num * sizeof(int16_t));
	::memcpy(_y, rhs.y(), _num * sizeof(int16_t));

	_colour = rhs.colour();

	return *this;
}

size_t Polygon::num() const {
	return _num;
}

const int16_t* Polygon::x() const {
	return _x;
}

const int16_t* Polygon::y() const {
	return _y;
}

const struct Colour& Polygon::colour() const {
	return _colour;
}
