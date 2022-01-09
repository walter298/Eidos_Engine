#ifndef TYPES
#define TYPES

#include <vector>
#include <iostream>
#include <stdarg.h>

#include "rendering.h"

class ed_Texture {
public:
	std::vector<std::vector<SDL_Texture*>> sheets; //spritesheet

	SDL_Rect ren; 
	SDL_Rect world;

	size_t sheetIndex = 0;
	size_t textureIndex = 0;

	int deltaCamX = 0, deltaCamY = 0;
	int deltaWorldX = 0, deltaWorldY = 0;

	Uint32 ID = 0;
};

class ed_Button {
public:
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	bool hovered(); 

	void(*reaction)() = NULL;

	Uint32 ID = 0;
};

class ed_Character {
public:
	ed_Texture texture;

	std::vector<void(*)()> backgroundMethods;

	Uint32 ID = 0;
};

class ed_Scene {
public:
	std::vector<ed_Texture> backgrounds;

	std::vector<SDL_Rect> surfaces;

	std::vector<ed_Button> buttons;

	std::vector<ed_Texture> foregroundObjects;

	std::vector<ed_Character> NPCS;

	ed_Character player;

	std::vector<void(*)()> backgroundMethods; //methods that run in the background

	bool executing = false;
};

void ed_ScaleTexture(ed_Texture& texture, int width, int height);

#endif