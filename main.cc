#include <cassert>
#include <csignal>
#include <ctime>
#include <iostream>
#include <locale>

#include <config.h>
#include <graphics.h>
#include <history.h>
#include <marshall.h>
#include <mutations.h>

#include <SDL.h>
#include <IMG_savepng.h>

int running = 1;

void sighandle(int t) {
	assert(t == SIGINT);
	running = 0;
}

void init_locale(void) {
	std::locale loc("");
	std::cout.imbue(loc);
	std::cerr.imbue(loc);
	std::cin.imbue(loc);
}

std::ostream& operator<<(std::ostream& s, const Config& c) {
	return s << "Config:" << std::endl
	         << "  WhiteBackground: " << c.whiteBG() << std::endl
	         << "  Width:           " << c.width() << std::endl
	         << "  Height:          " << c.height() << std::endl
	         << "  MaxPolySize:     " << c.maxPolySize() << std::endl;
}

void dump(const DNA& d) {
	std::cout << "DNA: " << d.score() << std::endl;
	for (uint32_t i = 0; i < d.num(); i++) {
		const Polygon& p(d[i]);
		for (uint32_t j = 0; j < p.num(); j++) {
			std::cout << "(" << p.x()[j] << ", " << p.y()[j] << "), ";
		}
		struct Colour c = p.colour();
		std::cout << "(" << (uint32_t)c.r << ", " << (uint32_t)c.g << ", " << (uint32_t)c.b << ", " << (uint32_t)c.a << ")" << std::endl;
	}
}

int main(void) { 
	Config c; 
	History h;
	DNA d;
	uint64_t x;

	signal(SIGINT, sighandle);

	init_locale();

	loadState("state.xml", h, c);

	SDL_Surface* ts = loadRGBA("target.jpg");

	c.setWidth(ts->w);
	c.setHeight(ts->h);

	if (h.num() == 0) {
		d = mutationAddPolygon(d, c);
		x = 0;
		h.update(d, 1);
	} else {
		d = h.dna(h.num() - 1);
		x = h.iter(h.num() - 1);
	}

	uint64_t nc = 0;
	time_t lastwrite = 0;
	DNA cand;
	SDL_Surface* cs = createSurface(c.width(), c.height());
	while (running) {
		x++;
		nc++;

		if (nc > 30 + d.num() / 2 && nc % 2 == 0 && d.num() < c.maxPolygons()) {
			cand = mutationAddPolygon(d, c);
		} else {
			do { 
				cand = mutate(d, c);
			} while (cand == d);
		}
	
		renderDNA(cs, cand, c);
		cand.setScore(calcScore(cs, ts));

		if (cand.score() < d.score()) {
			d = cand;
			h.update(d, x);
			if (time(NULL) - lastwrite > 5) {
				IMG_SavePNG("best.png", cs, 9);
				saveState("state.xml", h, c);
				lastwrite = time(NULL);
			}
			std::cout << "Replaced current with candidate. (NC: " << nc << ", Score: " << d.score() << ", Iter: " << x << ", Poly: " << d.num() << ")" << std::endl;
			nc = 0;
		}
	}

	IMG_SavePNG("best.png", cs, 9);
	saveState("state.xml", h, c);

	SDL_FreeSurface(cs);
	SDL_FreeSurface(ts);

	return 0;
}
