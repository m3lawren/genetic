#include <config.h>
#include <dna.h>
#include <mutations.h>

#include <cassert>
#include <cmath>
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
	if (a > b) { 
		assert(a <= b);
	}
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
	int32_t w = c.width() - 1;
	int32_t h = c.height() - 1;

	x[0] = randrange(0, w);
	y[0] = randrange(0, h);
	x[1] = min(max(0, x[0] + randrange(-3, 3)), w);
	y[1] = min(max(0, y[0] + randrange(-3, 3)), h);
	x[2] = min(max(0, x[0] + randrange(-3, 3)), w);
	y[2] = min(max(0, y[0] + randrange(-3, 3)), h);

	col.r = randrange(0, 255);
	col.g = randrange(0, 255);
	col.b = randrange(0, 255);
	col.a = randrange(c.minAlpha(), c.maxAlpha());

	size_t loc = randrange(0, p.size());
	p.insert(p.begin() + loc, Polygon(3, x, y, col));

	return DNA(p);
}

DNA mutationSwap(const DNA& d, const Config&) {
	if (d.num() <= 1) {
		return d;
	}

	std::vector<Polygon> p(d.polygons());
	size_t src = randrange(0, p.size() - 1);
	size_t dest = randrange(0, p.size() - 1);

	if (src == dest) {
		dest = (src + 1) % p.size();
	}

	Polygon t = p[src];
	if (src < dest) {
		p.insert(p.begin() + dest, t);
		p.erase(p.begin() + src);
	} else {
		p.erase(p.begin() + src);
		p.insert(p.begin() + dest, t);
	}
	return DNA(p);
}

DNA mutationVertexSwap(const DNA& d, const Config&) {
	if (d.num() < 1) {
		return d;
	}

	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	int owhich = which;
	while (true) {
		if (p[which].num() > 3) {
			break;
		}
		which++;
		which %= p.size();
		if (which == owhich) {
			return d;
		}
	}

	Polygon p1(p[which]);
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
	if (d.num() < 1) {
		return d;
	}

	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	int owhich = which;
	while (true) {
		if (p[which].num() > 3) {
			break;
		}
		which++;
		which %= p.size();
		if (which == owhich) {
			return d;
		}
	}

	Polygon p1(p[which]);
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
	if (d.num() < 1) {
		return d;
	}
	std::vector<Polygon> p(d.polygons());
	int which = randrange(0, p.size() - 1);
	int owhich = which;
	do {
		if (p[which].num() < c.maxDegree()) {
			break;
		}
		which++;
		which %= p.size();
		if (which == owhich) {
			return d;
		}
	} while (which != owhich);

	Polygon p1(p[which]);
	double location = (double)rand() / (double)RAND_MAX;
	int vert = randrange(0, p1.num() - 1);
	int16_t x0 = (int16_t)(location * (double)p1.x()[vert] + (1.0 - location) * (double)p1.x()[(vert + 1) % p1.num()] + randrange(-1, 1));
	int16_t y0 = (int16_t)(location * (double)p1.y()[vert] + (1.0 - location) * (double)p1.y()[(vert + 1) % p1.num()] + randrange(-1, 1));
	int16_t* x = new int16_t[p1.num() + 1];
	int16_t* y = new int16_t[p1.num() + 1];
	x0 = min(max(x0, 0), c.width() - 1);
	y0 = min(max(y0, 0), c.height() - 1);
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

DNA mutationPhysicalShift(const DNA& d, const Config& c) {
	if (d.num() < 1) {
		return d;
	}

	uint32_t whichp = randrange(0, d.num() - 1);
	std::vector<Polygon> p = d.polygons();
	Polygon p1 = p[whichp];
	uint32_t idx = randrange(0, p1.num() - 1);

	int16_t* x = new int16_t[p1.num()];
	int16_t* y = new int16_t[p1.num()];

	::memcpy(x, p1.x(), p1.num() * sizeof(int16_t));
	::memcpy(y, p1.y(), p1.num() * sizeof(int16_t));

	switch (randrange(0, 2)) {
		case 0:
			x[idx] = randrange(0, c.width() - 1);
			y[idx] = randrange(0, c.height() - 1);
			break;
		case 1:
			x[idx] = min(max(x[idx] + randrange(-(int64_t)c.width() / 4, c.width() / 4), 0), c.width() - 1);
			y[idx] = min(max(y[idx] + randrange(-(int64_t)c.height() / 4, c.height() / 4), 0), c.height() - 1);
			break;
		case 2:
			x[idx] = min(max(x[idx] + randrange(-3, 3), 0), c.width() - 1);
			y[idx] = min(max(y[idx] + randrange(-3, 3), 0), c.height() - 1);
			break;
	}

	p[whichp] = Polygon(p1.num(), x, y, p1.colour());
	delete[] x;
	delete[] y;
	return DNA(p);
}

DNA mutationColourShift(const DNA& d, const Config& c) {
	if (d.num() < 1) {
		return d;
	}

	uint32_t whichp = randrange(0, d.num() - 1);
	DNA result;
	std::vector<Polygon> p(d.polygons());
	Polygon p1(p[whichp]);
	Colour col;
	col.r = randrange(0, 255);
	col.g = randrange(0, 255);
	col.b = randrange(0, 255);
	col.a = randrange(c.minAlpha(), c.maxAlpha());
	p[whichp] = Polygon(p1.num(), p1.x(), p1.y(), col);
	return DNA(p);
}

void initMutations() {
	//REGISTER_MUT(mutationAddPolygon);
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
	DNA newd = d;
	int nmuts = 4 - log(randrange(1, 54));
	for (int i = 0; i < nmuts; i++) {
		newd = mutations[which](newd, c);
	}
	newd.setLastMutation(names[which]);
	return newd;
}

