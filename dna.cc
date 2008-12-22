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
