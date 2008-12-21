#include <polygon.h>

Polygon::Polygon() : _num(0), _x(NULL), _y(NULL) {
	_colour.r = _colour.g = _colour.b = _colour.a = 0;	
}

Polygon::~Polygon() {
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
