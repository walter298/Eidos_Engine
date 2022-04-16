#include "global.h"

std::vector<ed_RenderObject> ed_loadedTextures;

ed_Player c_Player;

bool ed_running = false;

SDL_Event ed_input;

const Uint8* ed_keyState = SDL_GetKeyboardState(NULL);

std::vector<std::thread> ed_runningThreads;