#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

class DNA;
struct SDL_Surface;

SDL_Surface* createSurface(int32_t width, int32_t height);
SDL_Surface* loadRGBA(const char*);

void renderDNA(SDL_Surface*, const DNA&, const Config&);
uint64_t calcScore(const SDL_Surface* a, const SDL_Surface* b);

#endif
