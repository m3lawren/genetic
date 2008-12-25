#include <dna.h>

#include <config.h>
#include <utils.h>

DNA::DNA() : _score((uint64_t)-1) {
	_polys.push_back(Polygon());
}

DNA::DNA(const std::vector<Polygon>& p) 
	: _polys(p), _score((uint64_t)-1) {
}
		
DNA::~DNA() {
}

size_t DNA::num() const {
	return _polys.size();
}

const Polygon& DNA::operator[](size_t idx) const {
	return _polys[idx];
}

const std::vector<Polygon>& DNA::polygons() const {
	return _polys;
}

uint64_t DNA::score() const {
	return _score;
}

void DNA::setScore(uint64_t v) {
	_score = v;
}

void DNA::mutate() {
	Config& c = Config::instance();
	bool changed = false;

	if (Utils::doMutate(c.mutPolyDelFreq())) {
		changed |= _mutatePolyDel();
	}

	if (Utils::doMutate(c.mutPolyAddFreq())) {
		changed |= _mutatePolyAdd();
	}

	if (Utils::doMutate(c.mutPolySwapFreq())) {
		 changed |= _mutatePolySwap();
	}
	
	for (size_t idx = 0; idx < _polys.size(); idx++) {
		changed |= _polys[idx].mutate();
	}
	
	if (changed) {
		_score = (uint64_t)-1;
	}
}

bool DNA::_mutatePolyDel() {
	if (_polys.size() == 0) {
		return false;
	}
	size_t loc = Utils::randRange(0, _polys.size() - 1);
	_polys.erase(_polys.begin() + loc);
	return true;
}

bool DNA::_mutatePolyAdd() {
	if (_polys.size() >= Config::instance().maxPolygons()) {
		return false;
	}
	size_t loc = Utils::randRange(0, _polys.size());
	_polys.insert(_polys.begin() + loc, Polygon());
	return true;
}

bool DNA::_mutatePolySwap() {
	if (_polys.size() <= 1) {
		return false;
	}

	size_t src = Utils::randRange(0, _polys.size() - 1);
	size_t dest = Utils::randRange(0, _polys.size() - 1);

	if (src == dest) {
		dest = (src + 1) % _polys.size();
	}

	Polygon t = _polys[src];
	if (src < dest) {
		_polys.insert(_polys.begin() + dest, t);
		_polys.erase(_polys.begin() + src);
	} else {
		_polys.erase(_polys.begin() + src);
		_polys.insert(_polys.begin() + dest, t);
	}
	return true;
}

bool operator==(const DNA& a, const DNA& b) {
	if (a.num() != b.num()) {
		return false;
	}
	if (a.score() != b.score()) {
		return false;
	}
	for (uint32_t idx = 0; idx < a.num(); idx++) {
		if (a[idx] != b[idx]) {
			return false;
		}
	}
	return true;
}

bool operator!=(const DNA& a, const DNA& b) {
	return !(a == b);
}
