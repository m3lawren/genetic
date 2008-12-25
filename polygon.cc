#include <polygon.h>

#include <config.h>
#include <cstring>
#include <utils.h>

bool Colour::mutate() {
	Config& c = Config::instance();
	bool ret = false;

	if (Utils::doMutate(c.mutColourFreq())) {
		r = Utils::randRange(0, 255);
		ret = true;
	}
	if (Utils::doMutate(c.mutColourFreq())) {
		g = Utils::randRange(0, 255);
		ret = true;
	}
	if (Utils::doMutate(c.mutColourFreq())) {
		b = Utils::randRange(0, 255);
		ret = true;
	}
	if (Utils::doMutate(c.mutColourFreq())) {
		a = Utils::randRange(c.minAlpha(), c.maxAlpha());
		ret = true;
	}
	return ret;
}

Polygon::Polygon() : _num(0), _x(NULL), _y(NULL) {
	Config& c = Config::instance();

	_num = _cap = 3;
	_x = new int16_t[_cap];
	_y = new int16_t[_cap];

	_x[0] = Utils::randRange(0, c.width() - 1);
	_y[0] = Utils::randRange(0, c.height() - 1);
	_x[1] = Utils::min(Utils::max(0, _x[0] + Utils::randRange(-3, 3)), c.width() - 1);
	_y[1] = Utils::min(Utils::max(0, _y[0] + Utils::randRange(-3, 3)), c.height() - 1);
	_x[2] = Utils::min(Utils::max(0, _x[0] + Utils::randRange(-3, 3)), c.width() - 1);
	_y[2] = Utils::min(Utils::max(0, _y[0] + Utils::randRange(-3, 3)), c.height() - 1);
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
	if (&rhs == this) {
		return *this;
	}
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

bool Polygon::mutate() {
	Config& c = Config::instance();
	bool ret = false;

	ret |= _colour.mutate();
	
	if (Utils::doMutate(c.mutPointDelFreq())) {
		ret |= _mutateDelPoint();
	}

	if (Utils::doMutate(c.mutPointAddFreq())) {
		ret |= _mutateAddPoint();
	}

	if (Utils::doMutate(c.mutPointSwapFreq())) {
		ret |= _mutateSwapPoint();
	}

	for (size_t idx = 0; idx < _num; idx++) {
		ret |= _mutatePoint(idx);
	}
	return ret;
}

bool Polygon::_mutatePoint(size_t idx) {
	Config& c = Config::instance();
	bool ret = false;
	if (Utils::doMutate(c.mutPointHugeFreq())) {
		_x[idx] = Utils::randRange(0, c.width() - 1);
		_y[idx] = Utils::randRange(0, c.height() - 1);
		ret = true;
	}
	if (Utils::doMutate(c.mutPointMedFreq())) {
		_x[idx] = Utils::min(Utils::max(_x[idx] + Utils::randRange(-(int64_t)c.width() / 4, c.width() / 4), 0), c.width() - 1);
		_y[idx] = Utils::min(Utils::max(_y[idx] + Utils::randRange(-(int64_t)c.height() / 4, c.height() / 4), 0), c.height() - 1);
		ret = true;
	}
	if (Utils::doMutate(c.mutPointSmallFreq())) {
		_x[idx] = Utils::min(Utils::max(_x[idx] + Utils::randRange(-3, 3), 0), c.width() - 1);
		_y[idx] = Utils::min(Utils::max(_y[idx] + Utils::randRange(-3, 3), 0), c.height() - 1);
		ret = true;
	}
	return ret;
}

bool Polygon::_mutateAddPoint() {
	if (_num >= Config::instance().maxDegree()) {
		return false;
	}
	if (_cap <= _num) {
		_cap = _num + 1;
		int16_t* nx = new int16_t[_cap];
		int16_t* ny = new int16_t[_cap];
		::memcpy(nx, _x, _num * sizeof(int16_t));
		::memcpy(ny, _y, _num * sizeof(int16_t));
		delete[] _x;
		delete[] _y;
		_x = nx;
		_y = ny;
	}

	size_t idx = Utils::randRange(0, _num);
	::memmove(_x + idx + 1, _x + idx, (_num - idx) * sizeof(int16_t));
	::memmove(_y + idx + 1, _y + idx, (_num - idx) * sizeof(int16_t));

	_num++;

	size_t prev = (idx + _num - 1) % _num;
	size_t next = (idx + 1) % _num;
	_x[idx] = ((int32_t)_x[prev] + (int32_t)_x[next]) / 2;
	_y[idx] = ((int32_t)_y[prev] + (int32_t)_y[next]) / 2;

	return true;
}

bool Polygon::_mutateDelPoint() {
	if (_num <= 3) {
		return false;
	}
	_num--;
	size_t idx = Utils::randRange(0, _num);
	::memmove(_x + idx, _x + idx + 1, (_num - idx) * sizeof(int16_t));
	return true;
}

bool Polygon::_mutateSwapPoint() {
	if (_num <= 1) {
		return false;
	}

	size_t src = Utils::randRange(0, _num - 1);
	size_t dest = Utils::randRange(0, _num - 1);
	if (dest == src) {
		dest = (dest + 1) % _num;
	}

	int16_t t = _x[src];
	_x[src] = _x[dest];
	_x[dest] = t;

	t = _y[src];
	_y[src] = _y[dest];
	_y[dest] = t;
	return true;
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
