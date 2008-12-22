#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL.h>

class DNA;

SDL_Surface* createSurface(Uint32 width, Uint32 height);

void renderDNA(SDL_Surface*, const DNA&);

#endif
