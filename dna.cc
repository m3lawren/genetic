#include <dna.h>

DNA::DNA() : _score((uint64_t)-1) {
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
