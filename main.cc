#include <cassert>
#include <csignal>
#include <ctime>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

#include <config.h>
#include <graphics.h>
#include <history.h>
#include <marshall.h>

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

void usage() {
	Config& c = Config::instance();
	std::cerr << "genetic [options]" << std::endl
	          << "  --black-bg, -b              Use a black background (default)." << std::endl
				 << "  --white-bg, -w              Use a white background." << std::endl
				 << "  --max-polygons, -p <num>    Set the maximum number of polygons (default: " << c.maxPolygons() << ")" << std::endl
				 << "  --max-poly-size, -s <num>   Set the maximum polygon size (default: " << c.maxPolySize() << ")" << std::endl
				 << "  --max-degree, -d <num>      Set the maximum degree of a polygon (default: " << c.maxDegree() << ")" << std::endl
				 << "  --min-alpha                 Set the minimum alpha value for a polygon (default: " << (uint32_t)c.minAlpha() << ")" << std::endl
				 << "  --max-alpha, -a <num>       Set the maximum alpha value for a polygon (default: " << (uint32_t)c.maxAlpha() << ")" << std::endl;
}

void parseOpts(int argc, char** argv, Config& c) {
	struct option longopts[] = {
		{"black-bg", 0, 0, 'b'},
		{"white-bg", 0, 0, 'w'},
		{"max-polygons", 1, 0, 'p'},
		{"max-poly-size", 1, 0, 's'},
		{"max-degree", 1, 0, 'd'},
		{"min-alpha", 1, 0, 0xff},
		{"max-alpha", 1, 0, 'a'},
		{"help", 0, 0, 'h'},
		{0, 0, 0, 0}
	};
	while (true) {
		int j = getopt_long(argc, argv, "bwp:s:d:a:h", longopts, NULL);
		std::istringstream s;
		if (j == -1) {
			break;
		}
		switch (j) {
			case 'w':
				c.setWhiteBG(true);
				break;
			case 'b':
				c.setWhiteBG(false);
				break;
			case 'p':
			{
				uint32_t v = c.maxPolygons();
				s.str(optarg);
				s >> v;
				c.setMaxPolygons(v);
				break;
			}
			case 's':
			{
				int32_t v = c.maxPolySize();
				s.str(optarg);
				s >> v;
				c.setMaxPolySize(v);
				break;
			}
			case 'd':
			{
				int32_t v = c.maxDegree();
				s.str(optarg);
				s >> v;
				c.setMaxDegree(v);
				break;
			}
			case 'a':
			{
				uint16_t v = c.maxAlpha();
				s.str(optarg);
				s >> v;
				c.setMaxAlpha(v);
				break;
			}
			case 0xff:
			{
				uint16_t v = c.minAlpha();
				s.str(optarg);
				s >> v;
				c.setMinAlpha(v);
				break;
			}
			case 'h':
			default:
				usage();
				::exit(1);
		}
	}
}

int main(int argc, char** argv) { 
	Config& c = Config::instance(); 
	History h;
	uint64_t x;

	signal(SIGINT, sighandle);

	init_locale();

	::srand(::time(NULL));

	std::cout << "Loading old state" << std::endl;
	/*loadState("state.xml", h, c);*/

	parseOpts(argc, argv, c);

	std::cout << "Loading target image" << std::endl;
	SDL_Surface* ts = loadRGBA("target.jpg");

	c.setWidth(ts->w);
	c.setHeight(ts->h);

	DNA d;
	if (h.num() == 0) {
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
	renderDNA(cs, d, c);
	d.setScore(calcScore(cs, ts));
	std::cout << "Starting main loop" << std::endl;
	while (running) {
		x++;
		nc++;

		cand = d;
		cand.mutate();
	
		renderDNA(cs, cand, c);
		cand.setScore(calcScore(cs, ts));

		if (cand.score() < d.score()) {
			d = cand;
			h.update(d, x);
			if (::time(NULL) - lastwrite > 5) {
				IMG_SavePNG("best.png", cs, 9);
				/*saveState("state.xml", h, c);*/
				lastwrite = ::time(NULL);
			}
			std::cout << "Replaced current with candidate. (NC: " << std::setw(4) << nc 
			          << ", Score: " << std::setw(13) << d.score() 
						 << ", Iter: " << std::setw(7) << x 
						 << ", Poly: " << d.num() << ")" << std::endl;
			nc = 0;
		}
	}

	IMG_SavePNG("best.png", cs, 9);
	/*saveState("state.xml", h, c);*/

	SDL_FreeSurface(cs);
	SDL_FreeSurface(ts);

	return 0;
}
