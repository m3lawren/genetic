#include <history.h>

History::History() {
}

History::~History() {
}

size_t History::num() const {
	return _g.size();
}

const DNA& History::dna(size_t idx) const {
	return _g[idx].first;
}

uint64_t History::iter(size_t idx) const {
	return _g[idx].second;
}

size_t History::update(const DNA& d, uint64_t iter) {
	std::pair<DNA, uint64_t> p(d, iter);
	if (num() == 0 || d.num() != _g.back().first.num()) {
		_g.push_back(p);
	} else {
		_g.back() = p;
	}
	return num();
}
