#ifndef RENDERING
#define RENDERING

#include "SDL.h"
#include "SDL_image.h"

extern SDL_Window* ed_mainWindow;

extern int ed_screenWidth;
extern int ed_screenHeight;

extern int ed_scaledWindowWidth;
extern int ed_scaledWindowHeight;

extern SDL_Renderer* ed_mainRenderer;

#endif