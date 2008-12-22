#include <history.h>

History::History() {
}

History::~History() {
}

size_t History::num() const {
	return _g.size();
}

const DNA& History::operator[](size_t idx) const {
	return _g[idx];
}

size_t History::update(const DNA& d) {
	if (d.num() != _g.back().num()) {
		_g.push_back(d);
	} else {
		_g.back() = d;
	}
	return num();
}
