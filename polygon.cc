#include <polygon.h>

#include <cstring>

Polygon::Polygon() : _num(0), _x(NULL), _y(NULL) {
}

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
	if (_x) {
		delete[] _x;
	}
	if (_y) {
		delete[] _y;
	}
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

bool operator==(const Polygon& a, const Polygon& b) {
	if (a.colour() != b.colour()) {
		return false;
	}
	if (a.num() != b.num()) {
		return false;
	}
	if (memcmp(a.x(), b.x(), a.num() * sizeof(int16_t)) != 0) {
		return false;
	}
	if (memcmp(a.y(), b.y(), a.num() * sizeof(int16_t)) != 0) {
		return false;
	}
	return true;
}

bool operator!=(const Polygon& a, const Polygon& b) {
	return !(a == b);
}

bool operator==(const struct Colour& a, const struct Colour& b) {
	return (memcmp(&a, &b, sizeof(struct Colour)) == 0);
}

bool operator!=(const struct Colour& a, const struct Colour& b) {
	return !(a == b);
}
