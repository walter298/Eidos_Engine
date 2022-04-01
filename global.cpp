#include "global.h"

std::vector<ed_Texture> ed_loadedTextures;

ed_Player c_Player;

bool ed_running = false;

std::vector<std::thread> ed_runningThreads;