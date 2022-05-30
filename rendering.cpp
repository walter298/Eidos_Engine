#include "rendering.h"

extern SDL_Window* ed_mainWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_RESIZABLE);

int ed_screenWidth = 1920;
int ed_screenHeight = 1080;

int ed_scaledWindowWidth;
int ed_scaledWindowHeight;

extern SDL_Renderer* ed_mainRenderer = SDL_CreateRenderer(ed_mainWindow, -1, SDL_RENDERER_ACCELERATED);

