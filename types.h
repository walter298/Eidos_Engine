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

	int centerX = 0, centerY = 0;
};

class ed_Texture {
public:
	std::vector<std::vector<SDL_Texture*>> sheets; //spritesheet

	//corresponds to spritesheet
	std::vector<std::vector<ed_Surface>> collisionGroups; 
	std::vector<std::vector<SDL_Rect>> renderGroups;

	SDL_Texture* currentTexture;

	void updateToCurrentTexture(size_t sheetIndex, size_t textureIndex);
	void updateToPosition(size_t sheetIndex, size_t textureIndex, int x , int y);

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

enum class ed_Dir {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class ed_RenderObject {
private:
	ed_Texture tex;
public:
	std::vector<void(*)()> backgroundMethods;

	ed_Surface surfaceBelow;

	void scaleTexture(ed_Texture& texture, int width, int height);

	ed_Surface getCollisionBox() {
		return this->tex.collisionGroups[this->tex.sheetIndex][this->tex.textureIndex];
	}

	SDL_Rect* getTextureBox() {
		return &this->tex.renderGroups[this->tex.sheetIndex][this->tex.textureIndex];
	}

	SDL_Texture* getCurrentTexture() {
		return this->tex.sheets[this->tex.sheetIndex][this->tex.textureIndex];
	}

	size_t getSheetIndex()
	{
		return this->tex.sheetIndex;
	}

	size_t getTextureIndex()
	{
		return this->tex.sheetIndex;
	}

	void makeTexture(ed_Texture texture)
	{
		this->tex = texture;
	}

	int getDeltaCamX()
	{
		return this->tex.deltaCamX;
	}

	int getDeltaWorldX()
	{
		return this->tex.deltaWorldX;
	}

	int getDeltaCamY()
	{
		return this->tex.deltaCamY;
	}

	int getDeltaWorldY()
	{
		return this->tex.deltaWorldY;
	}

	void setPos(int x, int y) {
		this->tex.updateToPosition(this->getSheetIndex(), this->getTextureIndex(), x, y);
	}

	void changeTexture(size_t sheetIndex, size_t textureIndex) 
	{
		this->tex.updateToCurrentTexture(sheetIndex, textureIndex);

		this->tex.currentTexture = this->tex.sheets[sheetIndex][textureIndex];

		this->tex.sheetIndex = sheetIndex;
		this->tex.textureIndex = textureIndex;
	}

	size_t getTextureGroupSize(int sheetIndex)
	{
		return this->tex.sheets[sheetIndex].size();
	}

	void worldMove(ed_Dir xDir, ed_Dir yDir)
	{
		int xWay = 0, yWay = 0;

		if (xDir == ed_Dir::RIGHT) {
			xWay = 1;
		} else if (xDir == ed_Dir::RIGHT) {
			yWay = -1;
		}

		if (yDir == ed_Dir::DOWN) {
			yWay = 1;
		} else {
			yWay = -1;
		} 

		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x1 += this->getDeltaWorldX() * xWay;
		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x2 += this->getDeltaWorldX() * xWay;

		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y1 += this->getDeltaWorldX() * yWay;
		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y2 += this->getDeltaWorldX() * yWay;
	}

	void camMove(ed_Dir xDir, ed_Dir yDir)
	{
		int xWay = 0, yWay = 0;

		if (xDir == ed_Dir::RIGHT) {
			xWay = 1;
		}
		else if (xDir == ed_Dir::RIGHT) {
			yWay = -1;
		}

		if (yDir == ed_Dir::DOWN) {
			yWay = 1;
		} else {
			yWay = -1;
		}

		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x1 += this->getDeltaCamX() * xWay;
		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x2 += this->getDeltaCamX() * xWay;

		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y1 += this->getDeltaCamX() * yWay;
		this->tex.collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y2 += this->getDeltaCamX() * yWay;
	}

	bool falling = false;

	bool xCamLocked = false, yCamLocked = false;

	Uint32 ID = 0;
};

class ed_Player : public ed_RenderObject {
public:
	virtual void jump();

	bool jumping = false;

	int health = 0;
};

#endif