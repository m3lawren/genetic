#include <config.h>
#include <dna.h>
#include <mutations.h>

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>

template<class T> void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

inline int32_t max(int32_t a, int32_t b) {
	return (a < b ? b : a);
}

inline int32_t min(int32_t a, int32_t b) {
	return (a < b ? a : b);
}

/* generates an int x s.t. a <= x <= b */
int32_t randrange(int32_t a, int32_t b) {
	assert(a <= b);
	assert(b + 1 - a <= RAND_MAX);
	if (a == b) {
		return a;
	}
	return (::rand() % (b + 1 - a)) + a;
}

typedef DNA (*mutation_t)(const DNA&, const Config&);
static std::vector<mutation_t> mutations;
static std::vector<std::string> names;

#define REGISTER_MUT(x) {mutations.push_back(x); names.push_back(#x);}

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

DNA mutationSwap(const DNA& d, const Config&) {
	std::vector<Polygon> p(d.polygons());
	int src = randrange(0, p.size() - 1);
	int dest = randrange(0, p.size() - 1);
	if (src == dest) {
		return d;
	}

	swap(p[src], p[dest]);

	return DNA(p);
}

DNA mutationVertexSwap(const DNA& d, const Config&) {
	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	Polygon p1 = p[which];
	if (p1.num() <= 3) {
		return d;
	}

	int16_t* x = new int16_t[p1.num()];
	int16_t* y = new int16_t[p1.num()];
	::memcpy(x, p1.x(), p1.num() * sizeof(int16_t));
	::memcpy(y, p1.y(), p1.num() * sizeof(int16_t));

	int src = randrange(0, p1.num() - 1);
	int dest = randrange(0, p1.num() - 1);
	if (src == dest) { 
		dest = (src + 1) % p1.num();
	}
	swap(x[src], x[dest]);
	swap(y[src], y[dest]);

	p[which] = Polygon(p1.num(), x, y, p1.colour());
	delete[] x;
	delete[] y;
	return DNA(p);
}

DNA mutationVertexDelete(const DNA& d, const Config&) {
	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	Polygon p1 = p[which];
	if (p1.num() <= 3) {
		return d;
	}

	int vert = randrange(0, p1.num() - 1);
	int16_t* x = new int16_t[p1.num() - 1];
	int16_t* y = new int16_t[p1.num() - 1];
	::memcpy(x, p1.x(), vert * sizeof(int16_t));
	::memcpy(y, p1.y(), vert * sizeof(int16_t));
	::memcpy(x + vert, p1.x() + vert + 1, (p1.num() - vert - 1) * sizeof(int16_t));
	::memcpy(y + vert, p1.y() + vert + 1, (p1.num() - vert - 1) * sizeof(int16_t));

	p[which] = Polygon(p1.num() - 1, x, y, p1.colour());
	delete[] x;
	delete[] y;
	return DNA(p);
}

DNA mutationVertexAdd(const DNA& d, const Config& c) {
	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	Polygon p1 = p[which];
	if (p1.num() >= c.maxDegree()) {
		return d;
	}

	double location = (double)rand() / (double)RAND_MAX;
	int vert = randrange(0, p1.num() - 1);
	int16_t x0 = (int16_t)(location * (double)p1.x()[vert] + (1.0 - location) * (double)p1.x()[(vert + 1) % p1.num()] + randrange(-1, 1));
	int16_t y0 = (int16_t)(location * (double)p1.y()[vert] + (1.0 - location) * (double)p1.y()[(vert + 1) % p1.num()] + randrange(-1, 1));
	int16_t* x = new int16_t[p1.num() + 1];
	int16_t* y = new int16_t[p1.num() + 1];
	vert++;
	vert %= p1.num();
	::memcpy(x, p1.x(), vert * sizeof(int16_t));
	::memcpy(y, p1.y(), vert * sizeof(int16_t));
	::memcpy(x + vert + 1, p1.x() + vert, (p1.num() - vert) * sizeof(int16_t));
	::memcpy(y + vert + 1, p1.y() + vert, (p1.num() - vert) * sizeof(int16_t));
	x[vert] = x0;
	y[vert] = y0;

	p[which] = Polygon(p1.num() + 1, x, y, p1.colour());
	delete[] x;
	delete[] y;
	return DNA(p);
}

DNA mutationVertexReplace(const DNA& d, const Config& c) {
	DNA result;
	result = mutationVertexDelete(d, c);
	if (result == d) {
		result = mutationVertexAdd(d, c);
		result = mutationVertexDelete(result, c);
	} else {
		result = mutationVertexAdd(result, c);
	}
	return result;
}

DNA mutationDelete(const DNA& d, const Config&) {
	if (d.num() <= 1) {
		return d;
	}
	int which = randrange(0, d.num() - 1);
	std::vector<Polygon> p(d.polygons());
	p.erase(p.begin() + which);
	return DNA(p);
}

DNA _mutationShift(const DNA& d, uint32_t whichp, uint32_t idx, bool x, int32_t minv, int32_t maxv, int32_t maxchange) {
	if (d.num() == 0) {
		return d;
	}
	std::vector<Polygon> p(d.polygons());
	Polygon p1(p[whichp]);
	int16_t change = randrange(-maxchange, maxchange);
	int16_t newv = (x?p1.x():p1.y())[idx] + change;
	if (newv < minv) {
		newv = minv;
	} else if (newv > maxv) {
		newv = maxv;
	}

	int16_t* v = new int16_t[p1.num()];
	::memcpy(v, (x?p1.x():p1.y()), p1.num() * sizeof(int16_t));
	v[idx] = newv;
	p[whichp] = Polygon(p1.num(), (x?v:p1.x()), (x?p1.y():v), p1.colour());
	delete[] v;
	return DNA(p);
}

DNA _mutationColour(const DNA& d, uint32_t whichp, uint32_t idx, int32_t minv, int32_t maxv, int32_t maxchange) {
	std::vector<Polygon> p(d.polygons());
	Polygon p1(p[whichp]);
	uint8_t* v = NULL;
	Colour c = p1.colour();
	if (idx == 0) {
		v = &c.r;
	} else if (idx == 1) {
		v = &c.g;
	} else if (idx == 2) {
		v = &c.b;
	} else if (idx == 3) {
		v = &c.a;
	} else {
		return d;
	}

	int32_t change = randrange(-maxchange, maxchange);
	if ((int32_t)*v < minv - change) {
		*v = minv;
	} else if ((int32_t)*v > maxv - change) {
		*v = maxv;
	} else {
		*v += change;
	}

	p[whichp] = Polygon(p1.num(), p1.x(), p1.y(), c);
	return DNA(p);
}

DNA mutationPhysicalShift(const DNA& d, const Config& c) {
	uint32_t whichp = randrange(0, d.num() - 1);
	uint32_t idx = randrange(0, d[whichp].num() - 1);

	DNA result = _mutationShift(d, whichp, idx, true, 0, c.width() - 1, c.deltaCoord());
	return _mutationShift(result, whichp, idx, false, 0, c.height() - 1, c.deltaCoord());
}

DNA mutationColourShift(const DNA& d, const Config& c) {
	uint32_t whichp = randrange(0, d.num() - 1);
	DNA result;
	result = _mutationColour(d, whichp, 0, 0, 255, c.deltaColour());
	result = _mutationColour(result, whichp, 1, 0, 255, c.deltaColour());
	result = _mutationColour(result, whichp, 2, 0, 255, c.deltaColour());
	result = _mutationColour(result, whichp, 3, c.minAlpha(), c.maxAlpha(), c.deltaColour());
	return result;
}

void initMutations() {
	REGISTER_MUT(mutationSwap);
	REGISTER_MUT(mutationDelete);
	REGISTER_MUT(mutationVertexSwap);
	REGISTER_MUT(mutationVertexDelete);
	REGISTER_MUT(mutationVertexAdd);
	REGISTER_MUT(mutationVertexReplace);
	REGISTER_MUT(mutationPhysicalShift);
	REGISTER_MUT(mutationColourShift);
}

DNA mutate(const DNA& d, const Config& c) {
	if (mutations.size() == 0) {
		initMutations();
	}
	int which = randrange(0, mutations.size() - 1);
	DNA newd = mutations[which](d, c);
	newd.setLastMutation(names[which]);
	return newd;
}

