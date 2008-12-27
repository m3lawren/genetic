#include <SDL.h>
#include <IMG_savepng.h>
#include <SDL_image.h>

#include <getopt.h>

#include <iostream>

SDL_Surface* desired = NULL;

SDL_Surface* createSurface(int32_t width, int32_t height) { 
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface* s;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff << 16;
	gmask = 0xff << 8;
	bmask = 0xff;
#else
	rmask = 0xff;
	gmask = 0xff << 8;
	bmask = 0xff << 16;
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

void usage() {
	std::cout << "merge [options] files..." << std::endl
	          << "  --output, -o  Specifies the output file" << std::endl;
}

int main(int argc, char** argv) {
	uint32_t num;
	const char* output = "merge.png";
	SDL_Surface* base = NULL;
	SDL_Surface* temp = NULL;

	struct option longopts[] = {
		{ "--output", 1, 0, 'o' },
		{ "--help", 0, 0, 'h' },
		{0, 0, 0, 0}
	};
	while (true) {
		int j = getopt_long(argc, argv, "o:h", longopts, NULL);
		if (j == -1) {
			break;
		}
		switch (j) {
			case 'o':
				output = optarg;
				break;
			case 'h':
				usage();
				return 0;
			default:
				usage();
				return 1;
		}
	}

	num = argc - optind;
	if (num < 2) {
		usage();
		return 1;
	}

	std::cout << "Merging " << num << " images." << std::endl;

	for (int32_t i = optind; i < argc; i++) {
		const char* file = argv[i];
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

	IMG_SavePNG(output, base, 9);
	SDL_FreeSurface(base);
	std::cout << "Output saved as '" << output << "'." << std::endl;
	return 0;
}
