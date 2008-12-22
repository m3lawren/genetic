#include <config.h>
#include <dna.h>
#include <mutations.h>

#include <cassert>
#include <cstdlib>

inline int32_t max(int32_t a, int32_t b) {
	return (a < b ? b : a);
}

inline int32_t min(int32_t a, int32_t b) {
	return (a < b ? a : b);
}

/* generates an int x s.t. a <= x <= b */
uint32_t randrange(uint32_t a, uint32_t b) {
	assert(a <= b);
	return (::rand() % (b - a)) + a;
}

DNA mutationAddPolygon(const DNA& d, const Config& c) {
	std::vector<Polygon> p(d.polygons());
	int16_t x[3], y[3];
	struct Colour col;
	int32_t msz = c.maxPolySize();
	int32_t w = c.width() - 1;
	int32_t h = c.height() - 1;

	x[0] = randrange(0, w);
	y[0] = randrange(0, h);
	x[1] = randrange(max(0, x[0] - msz), min(w, x[0] + msz));
	y[1] = randrange(max(0, y[0] - msz), min(h, y[0] + msz));
	x[2] = randrange(max(0, (x[0] + x[1]) / 2 - msz), min(w, (x[0] + x[1]) / 2 + msz));
	y[2] = randrange(max(0, (y[0] + y[1]) / 2 - msz), min(h, (y[0] + y[1]) / 2 + msz));

	col.r = randrange(0, 255);
	col.g = randrange(0, 255);
	col.b = randrange(0, 255);
	col.a = randrange(15, 255);

	p.push_back(Polygon(3, x, y, col));

	return DNA(p);
}
