#include <SDL.h>
#include <IMG_savepng.h>
#include <SDL_image.h>

#include <iostream>

SDL_Surface* desired = NULL;

SDL_Surface* createSurface(int32_t width, int32_t height) { 
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface* s;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	/*rmask = 0xff << 24;
	gmask = 0xff << 16;
	bmask = 0xff << 8;
	amask = 0xff;*/
	rmask = 0xff << 16;
	gmask = 0xff << 8;
	bmask = 0xff;
#else
	rmask = 0xff;
	gmask = 0xff << 8;
	bmask = 0xff << 16;
	//amask = 0xff << 24;
#endif
	amask = 0x00;

	s = SDL_CreateRGBSurface(SDL_SRCALPHA, width, height, 24, rmask, gmask, bmask, amask);

	if (s == NULL) {
		std::cerr << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
		::exit(1);
	}

	return s;
}

SDL_Surface* loadRGB(const char* f) {
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

int main(int argc, char** argv) {
	uint32_t num = argc - 1;
	SDL_Surface* base = NULL;
	SDL_Surface* temp = NULL;

	//desired = createSurface(1, 1);

	std::cout << "Merging " << num << " images." << std::endl;

	for (uint32_t i = 0; i < num; i++) {
		const char* file = argv[i + 1];
		std::cout << "Processing '" << file << "'." << std::endl;
		temp = loadRGB(file);
		if (!base) {
			base = temp;
		} else {
			if (base->w != temp->w || base->h != temp->h) {
				std::cerr << "Dimensions of '" << file << "' do not match previous images." << std::endl;
				::exit(1);
			}
			SDL_SetAlpha(temp, SDL_SRCALPHA, 255 / (i + 1));
			SDL_SetAlpha(base, SDL_SRCALPHA, 255 / (i + 1));
			SDL_BlitSurface(temp, NULL, base, NULL);
			SDL_FreeSurface(temp);
		}
	}

	IMG_SavePNG("merge.png", base, 9);
	SDL_FreeSurface(base);
	std::cout << "Output saved as 'merge.png'." << std::endl;
	return 0;
}
