#include <SDL.h>
#include <SDL_image.h>
#include <IMG_savepng.h>
#include <SDL_gfxPrimitives.h>

#include <graphics.h>

#include <iostream>
#include <locale>

void init_locale(void) {
	std::locale loc("");
	std::locale::global(loc);
	std::cout.imbue(loc);
	std::cerr.imbue(loc);
	std::cin.imbue(loc);
}

int main(void) { 
	init_locale();

	SDL_Surface* s = createSurface(200, 200);
	std::cout << std::hex << SDL_MapRGBA(s->format, 0, 127, 255, 255) << std::endl;
	std::cout << std::hex << 0x007fffff << std::endl;
	SDL_LockSurface(s);
	boxRGBA(s, 0, 0, 200, 200, 0, 127, 255, 255);
	SDL_UnlockSurface(s);
	IMG_SavePNG("test.png", s, -1);
	SDL_FreeSurface(s);	

	SDL_RWops* op = SDL_RWFromFile("test.png", "rb");
	s = IMG_LoadPNG_RW(op);
	if (!s) { 
		std::cerr << "IMG_LoadPNG_RW failed: " << SDL_GetError() << std::endl;
		::exit(1);
	}
	SDL_FreeSurface(s);
	return 0;
}
