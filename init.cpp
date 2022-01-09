#include "init.h"

void ed_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error: SDL did not initialize correctly.\n";

		exit(-1);
	}

	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);

	ed_screenWidth = mode.w;
	ed_screenHeight = mode.h;
}