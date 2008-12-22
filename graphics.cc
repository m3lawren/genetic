#include <graphics.h>
#include <dna.h>

#include <iostream>

#include <SDL_gfxPrimitives.h>

SDL_Surface* createSurface(Uint32 width, Uint32 height) { 
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

Uint32 colourToInt(struct Colour c) {
	Uint32 r = c.r, g = c.g, b = c.b, a = c.a;
	return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff);
}

void renderDNA(SDL_Surface* s, const DNA& d) {
	rectangleColor(s, 0, 0, s->w, s->h, 0x000000ff);		
	for (size_t i = 0; i < d.num(); i++) {
		polygonColor(s, d[i].x(), d[i].y(), d[i].num(), colourToInt(d[i].colour()));
	}
}
