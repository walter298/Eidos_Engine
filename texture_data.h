#ifndef TEXTURE_DATA
#define TEXTURE_DATA

#include <fstream>

#include "types.h"
#include "global.h"
#include "data_util.h"

void ed_enterTextureData(ed_Texture& texture, std::string fileName);
void ed_enterTextureCollision(std::string path);

#endif