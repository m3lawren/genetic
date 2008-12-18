#include <graphics.h>

#include <iostream>

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
