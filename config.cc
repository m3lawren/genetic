#include <config.h>

#include <cassert>

#define FREQ_LOW 1000
#define FREQ_MED 500

Config::Config() 
	: _whiteBG(false), _w(0), _h(0), 
	  _mp(150), _mpsz(40), _md(8),
	  _mxa(63), _mna(7), 
	  _mplaf(FREQ_MED), _mpldf(FREQ_LOW), _mplsf(FREQ_LOW),
	  _mcf(FREQ_LOW),
	  _mpthf(FREQ_LOW), _mptmf(FREQ_LOW),  _mptsf(FREQ_LOW), _mptaf(FREQ_LOW), _mptdf(FREQ_LOW), _mptwf(FREQ_LOW) {
}

Config& Config::instance() {
	static Config c;
	return c;
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

uint32_t Config::maxDegree() const {
	return _md;
}

uint8_t Config::maxAlpha() const {
	return _mxa;
}

uint8_t Config::minAlpha() const {
	return _mna;
}

uint32_t Config::mutPolyAddFreq() const {
	return _mplaf;
}

uint32_t Config::mutPolyDelFreq() const {
	return _mpldf;
}

uint32_t Config::mutPolySwapFreq() const {
	return _mplsf;
}

uint32_t Config::mutColourFreq() const {
	return _mcf;
}

uint32_t Config::mutPointHugeFreq() const {
	return _mpthf;
}

uint32_t Config::mutPointMedFreq() const {
	return _mptmf;
}

uint32_t Config::mutPointSmallFreq() const {
	return _mptsf;
}

uint32_t Config::mutPointAddFreq() const {
	return _mptaf;
}

uint32_t Config::mutPointDelFreq() const {
	return _mptdf;
}

uint32_t Config::mutPointSwapFreq() const {
	return _mptwf;
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

void Config::setMutPolyAddFreq(uint32_t v) {
	_mplaf = v;
}

void Config::setMutPolyDelFreq(uint32_t v) {
	_mpldf = v;
}

void Config::setMutPolySwapFreq(uint32_t v) {
	_mplsf = v;
}

void Config::setMutColourFreq(uint32_t v) {
	_mcf = v;
}

void Config::setMutPointHugeFreq(uint32_t v) {
	_mpthf = v;
}

void Config::setMutPointMedFreq(uint32_t v) {
	_mptmf = v;
}

void Config::setMutPointSmallFreq(uint32_t v) {
	_mptsf = v;
}

void Config::setMutPointAddFreq(uint32_t v) {
	_mptaf = v;
}

void Config::setMutPointDelFreq(uint32_t v) {
	_mptdf = v;
}

void Config::setMutPointSwapFreq(uint32_t v) {
	_mptwf = v;
}
