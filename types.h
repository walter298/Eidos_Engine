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

class ed_Scene; 
class ed_RenderObjectCreator;

class ed_RenderObject {
protected:
	std::vector<std::vector<SDL_Texture*>> sheets; //spritesheet

	//corresponds to spritesheet
	std::vector<std::vector<ed_Surface>> collisionGroups;
	std::vector<std::vector<SDL_Rect>> renderGroups;

	SDL_Texture* currentTexture;

	void updateToCurrentTexture(size_t sheetIndex, size_t textureIndex)
	{
		//our current render position
		int CRX = this->renderGroups[this->sheetIndex][this->textureIndex].x;
		int CRY = this->renderGroups[this->sheetIndex][this->textureIndex].y;

		//set new texture to be at current position
		this->renderGroups[sheetIndex][textureIndex].x = CRX;
		this->renderGroups[sheetIndex][textureIndex].y = CRY;

		ed_Surface* currentSurface = &this->collisionGroups[this->sheetIndex][this->textureIndex];
		ed_Surface* updatedSurface = &this->collisionGroups[sheetIndex][textureIndex];

		//our current collision position
		size_t centerX = currentSurface->centerX;
		size_t centerY = currentSurface->centerY;

		int distX = updatedSurface->x2 - centerY;
		int distY = updatedSurface->y2 - centerY;

		//set collision constants to be at current position
		updatedSurface->centerX = centerX;
		updatedSurface->centerY = centerY;
		updatedSurface->x1 = centerX - distX;
		updatedSurface->y1 = centerY - distY;
		updatedSurface->x2 = centerX + distX;
		updatedSurface->y2 = centerY + distY;
	}

	void updateToPosition(size_t sheetIndex, size_t textureIndex, int x, int y)
	{
		auto* collisionBox = &this->collisionGroups[sheetIndex][textureIndex];
		auto* texture = &this->renderGroups[sheetIndex][textureIndex];

		int xDistance = collisionBox->centerX - collisionBox->x1;
		int yDistance = collisionBox->centerY - collisionBox->y1;

		collisionBox->centerX = x;
		collisionBox->centerY = y - yDistance;
		collisionBox->x1 = x - xDistance;
		collisionBox->y1 = y - yDistance * 2;
		collisionBox->x2 = x + xDistance;
		collisionBox->y2 = y;

		texture->x = collisionBox->x1;
		texture->y = collisionBox->y1;
	}

	size_t sheetIndex = 0;
	size_t textureIndex = 0;

	size_t deltaTexture = 1;

	int deltaCamX = 0, deltaCamY = 0;
	int deltaWorldX = 0, deltaWorldY = 0;
public:
	std::vector<void(*)()> backgroundMethods;

	ed_Surface surfaceBelow;

	void scale(int windowWidth, int windowHeight)
	{
		auto scaledRelativePosition = [](int p1, int m1, int m2) {
			double relativePosition = 1.0 * p1 / m1;

			return relativePosition * m2;
		};

		for (size_t i = 0; i < sheets.size(); i++) {
			for (size_t j = 0; j < sheets[i].size(); j++) {
				renderGroups[i][j].x = scaledRelativePosition(renderGroups[i][j].x, 1920, windowWidth);
				renderGroups[i][j].y = scaledRelativePosition(renderGroups[i][j].y, 1080, windowHeight);

				renderGroups[i][j].w = scaledRelativePosition(renderGroups[i][j].w, 1920, windowWidth);
				renderGroups[i][j].h = scaledRelativePosition(renderGroups[i][j].h, 1080, windowHeight);

				collisionGroups[i][j].y1 = scaledRelativePosition(collisionGroups[i][j].y1, 1920, windowWidth);
				collisionGroups[i][j].y2 = scaledRelativePosition(collisionGroups[i][j].y2, 1080, windowHeight);

				collisionGroups[i][j].x1 = scaledRelativePosition(collisionGroups[i][j].x1, 1920, windowWidth);
				collisionGroups[i][j].x2 = scaledRelativePosition(collisionGroups[i][j].x2, 1080, windowHeight);

				collisionGroups[i][j].centerX = scaledRelativePosition(collisionGroups[i][j].centerX, 1920, windowWidth);
				collisionGroups[i][j].centerY = scaledRelativePosition(collisionGroups[i][j].centerY, 1080, windowHeight);
			}
		}

		deltaCamX = scaledRelativePosition(deltaCamX, 1920, windowWidth);
		deltaCamY = scaledRelativePosition(deltaCamY, 1080, windowHeight);

		deltaWorldX = scaledRelativePosition(deltaWorldX, 1920, windowWidth);
		deltaWorldY = scaledRelativePosition(deltaWorldY, 1080, windowHeight);
	}

	ed_Surface getCollisionBox() {
		return this->collisionGroups[this->sheetIndex][this->textureIndex];
	}

	SDL_Rect* getTextureBox() {
		return &this->renderGroups[this->sheetIndex][this->textureIndex];
	}

	SDL_Texture* getCurrentTexture() {
		return this->sheets[this->sheetIndex][this->textureIndex];
	}

	size_t getSheetIndex()
	{
		return this->sheetIndex;
	}

	size_t getTextureIndex()
	{
		return this->textureIndex;
	}

	int getDeltaCamX()
	{
		return this->deltaCamX;
	}

	int getDeltaWorldX()
	{
		return this->deltaWorldX;
	}

	int getDeltaCamY()
	{
		return this->deltaCamY;
	}

	int getDeltaWorldY()
	{
		return this->deltaWorldY;
	}

	void setPos(int x, int y) {
		this->updateToPosition(this->getSheetIndex(), this->getTextureIndex(), x, y);
	}

	void setRenderPos(int x, int y)
	{
		SDL_Rect* rect = &this->renderGroups[this->getSheetIndex()][this->getTextureIndex()];

		rect->x = x;
		rect->y = y;
	}

	void changeTexture(size_t sheetIndex, size_t textureIndex) 
	{
		this->updateToCurrentTexture(sheetIndex, textureIndex);

		this->sheetIndex = sheetIndex;
		this->textureIndex = textureIndex;

		this->currentTexture = this->sheets[sheetIndex][textureIndex];
	}

	size_t getTextureGroupSize(int sheetIndex)
	{
		return this->sheets[sheetIndex].size();
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
		} else if (yDir == ed_Dir::UP) {
			yWay = -1;
		} 

		this->collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x1 += this->getDeltaWorldX() * xWay;
		this->collisionGroups[this->getSheetIndex()][this->getTextureIndex()].x2 += this->getDeltaWorldX() * xWay;

		this->collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y1 += this->getDeltaWorldX() * yWay;
		this->collisionGroups[this->getSheetIndex()][this->getTextureIndex()].y2 += this->getDeltaWorldX() * yWay;
	}

	void camMove(ed_Dir xDir, ed_Dir yDir)
	{
		int xWay = 0, yWay = 0;

		if (xDir == ed_Dir::RIGHT) {
			xWay = 1;
		} else if (xDir == ed_Dir::LEFT) {
			xWay = -1;
		}

		if (yDir == ed_Dir::DOWN) {
			yWay = 1;
		} else if (yDir == ed_Dir::UP) {
			yWay = -1;
		}

		this->renderGroups[this->getSheetIndex()][this->getTextureIndex()].x += this->getDeltaCamX() * xWay;
		this->renderGroups[this->getSheetIndex()][this->getTextureIndex()].y += this->getDeltaCamX() * yWay;
	}

	bool falling = false;

	bool xCamLocked = false, yCamLocked = false;

	friend class ed_Scene;
	friend class ed_RenderObjectCreator;

	Uint32 ID = 0;
};

class ed_Player : public ed_RenderObject {
public:
	virtual void jump();

	bool jumping = false;

	int health = 0;
};

#endif