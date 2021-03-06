#include <config.h>

#include <cassert>

Config::Config() 
	: _whiteBG(false), _w(0), _h(0), 
	  _mp(150), _mpsz(40), _dcr(5), _md(8),
	  _mxa(191), _mna(15), _dcl(15) {
}

Config::~Config() {
}

bool Config::whiteBG() const {
	return _whiteBG;
}

uint32_t Config::width() const {
	return _w;
}

uint32_t Config::height() const {
	return _h;
}

uint32_t Config::maxPolygons() const {
	return _mp;
}

uint32_t Config::maxPolySize() const {
	return _mpsz;
}

uint32_t Config::deltaCoord() const {
	return _dcr;
}

uint32_t Config::maxDegree() const {
	return _md;
}

uint8_t Config::maxAlpha() const {
	return _mxa;
}

uint8_t Config::minAlpha() const {
	return _mna;
}

uint8_t Config::deltaColour() const {
	return _dcl;
}

void Config::setWhiteBG(bool v) {
	_whiteBG = v;
}

void Config::setWidth(uint32_t v) {
	assert(v > 0);
	_w = v;
}

void Config::setHeight(uint32_t v) {
	assert(v > 0);
	_h = v;
}

void Config::setMaxPolygons(uint32_t v) {
	_mp = v;
}

void Config::setMaxPolySize(uint32_t v) {
	assert(v > 0);
	_mpsz = v;
}

void Config::setDeltaCoord(uint32_t v) {
	assert(v > 0);
	_dcr = v;
}

void Config::setMaxDegree(uint32_t v) {
	_md = v;
}

void Config::setMaxAlpha(uint8_t v) {
	if (v < minAlpha()) return;
	_mxa = v;
}

void Config::setMinAlpha(uint8_t v) {
	if (v > maxAlpha()) return;
	_mna = v;
}

void Config::setDeltaColour(uint8_t v) {
	_dcl = v;
}
