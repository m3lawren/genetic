#include <dna.h>

DNA::DNA() {
}

DNA::DNA(const std::vector<Polygon>& p) 
	: _polys(p) {
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
