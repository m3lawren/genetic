#include <polygon.h>

#include <cstring>

Polygon::Polygon(size_t num, const uint32_t* x, const uint32_t* y, struct Colour colour) 
	: _num(num), _colour(colour) {
	_x = new uint32_t[_num];
	_y = new uint32_t[_num];

	::memcpy(_x, x, _num * sizeof(uint32_t));
	::memcpy(_y, y, _num * sizeof(uint32_t));
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
		_x = new uint32_t[rhs.num()];
		_y = new uint32_t[rhs.num()];
		_cap = rhs.num();
	}

	_num = rhs.num();
	::memcpy(_x, rhs.x(), _num * sizeof(uint32_t));
	::memcpy(_y, rhs.y(), _num * sizeof(uint32_t));

	_colour = rhs.colour();

	return *this;
}

size_t Polygon::num() const {
	return _num;
}

const uint32_t* Polygon::x() const {
	return _x;
}

const uint32_t* Polygon::y() const {
	return _y;
}

const struct Colour& Polygon::colour() const {
	return _colour;
}
