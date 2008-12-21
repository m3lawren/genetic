#include <dna.h>

DNA::DNA() {
}

DNA::~DNA() {
}

size_t DNA::num() const {
	return _polys.size();
}

Polygon& DNA::poly(size_t idx) {
	return _polys[idx];
}

Polygon& DNA::operator[](size_t idx) {
	return _polys[idx];
}
