#include <config.h>
#include <dna.h>
#include <graphics.h>

#include <cassert>
#include <iostream>

#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>

SDL_Surface* createSurface(int32_t width, int32_t height) { 
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface* s;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff << 24;
	gmask = 0xff << 16;
	bmask = 0xff << 8;
	amask = 0xff;
#else
	rmask = 0xff;
	gmask = 0xff << 8;
	bmask = 0xff << 16;
	amask = 0xff << 24;
#endif

	s = SDL_CreateRGBSurface(SDL_SRCALPHA, width, height, 32, rmask, gmask, bmask, amask);

	if (s == NULL) {
		std::cerr << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
		::exit(1);
	}

	return s;
}

SDL_Surface* loadRGBA(const char* f) {
	SDL_Surface* l;
	SDL_Surface* s;

	l = IMG_Load(f);
	if (!l) {
		std::cerr << "IMG_Load failed: " << SDL_GetError() << std::endl;
		::exit(1);
	}

	s = createSurface(l->w, l->h);
	SDL_BlitSurface(l, NULL, s, NULL);
	
	SDL_FreeSurface(l);
	return s;
}

Uint32 colourToInt(struct Colour c) {
	Uint32 r = c.r, g = c.g, b = c.b, a = c.a;
	return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff);
}

void renderDNA(SDL_Surface* s, const DNA& d, const Config& c) {
	Uint32 bg = c.whiteBG() ? 0xffffffff : 0x000000ff;
	assert(c.width() == (uint32_t)s->w && c.height() == (uint32_t)s->h);
	boxColor(s, 0, 0, s->w - 1, s->h - 1, bg);	/* SDL_FillRect? */
	for (size_t i = 0; i < d.num(); i++) {
		filledPolygonColor(s, d[i].x(), d[i].y(), d[i].num(), colourToInt(d[i].colour()));
	}
}

uint64_t calcScore(const SDL_Surface* a, const SDL_Surface* b) {
	uint64_t sc = 0;
	uint8_t* p1 = (uint8_t*)a->pixels;
	uint8_t* p2 = (uint8_t*)b->pixels;
	assert(a->format->BitsPerPixel == b->format->BitsPerPixel);
	for (int64_t idx = 0; idx < 4 * a->w * a->h; idx++) {
		sc += ((uint32_t)p1[idx] - (uint32_t)p2[idx]) * ((uint32_t)p1[idx] - (uint32_t)p2[idx]);
	}
	return sc;
}
