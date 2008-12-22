#include <config.h>

#include <cassert>

Config::Config() {
}

Config::~Config() {
}

bool Config::whiteBG() const {
	return _whiteBG;
}

int32_t Config::width() const {
	return _w;
}

int32_t Config::height() const {
	return _h;
}

int32_t Config::maxPolySize() const {
	return _mpsz;
}

void Config::setWhiteBG(bool v) {
	_whiteBG = v;
}

void Config::setWidth(int32_t v) {
	assert(v > 0);
	_w = v;
}

void Config::setHeight(int32_t v) {
	assert(v > 0);
	_h = v;
}

void Config::setMaxPolySize(int32_t v) {
	assert(v > 0);
	_mpsz = v;
}
