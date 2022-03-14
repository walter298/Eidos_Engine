#ifndef TYPES
#define TYPES

#include <vector>
#include <iostream>
#include <thread>
#include <stdarg.h>

#include "rendering.h"

class ed_Surface {
public:
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
};

class ed_Texture {
public:
	std::vector<std::vector<SDL_Texture*>> sheets; //spritesheet

	//corresponds to spritesheet
	std::vector<std::vector<ed_Surface>> collisionGroups; 
	std::vector<std::vector<SDL_Rect>> renderGroups;

	size_t sheetIndex = 0;
	size_t textureIndex = 0;

	size_t deltaTexture = 1;

	int deltaCamX = 0, deltaCamY = 0;
	int deltaWorldX = 0, deltaWorldY = 0;

	int ID = 0;
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
	ed_Texture tex;

	std::vector<void(*)()> backgroundMethods;

	void updateTexture(int sheetIndex, int textureIndex);

	ed_Surface surfaceBelow = { 0, 0, 0, 0 };

	bool falling = false;

	Uint32 ID = 0;
};

class ed_Player : public ed_Character {
public:
	virtual void jump();

	bool jumping = false;

	int health = 0;
};

class ed_Scene {
public:
	std::vector<ed_Texture> backgrounds;

	std::vector<ed_Surface> surfaces;

	std::vector<ed_Button> buttons;

	std::vector<ed_Texture> foregroundObjects;

	std::vector<ed_Character> NPCS;

	bool containsPlayer = true;

	std::vector<void(*)()> backgroundMethods;

	bool executing = false;
};

void ed_ScaleTexture(ed_Texture& texture, int width, int height);

#endif