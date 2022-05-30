#ifndef TYPES
#define TYPES

#include <vector>
#include <iostream>
#include <thread>
#include <stdarg.h>

#include "rendering.h"
#include "object_id_defs.h"

class ed_Surface {
public:
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	int centerX = 0, centerY = 0;

	//checking equivalence
	bool operator == (const ed_Surface& s)
	{
		if (this->x1 == s.x1 && this->y1 == s.y1 &&
			this->x2 == s.x2 && this->y2 == s.y2) 
		{
			return true;
		} else {
			return false;
		}
	}

	bool operator != (const ed_Surface& s) {
		if (this->x1 == s.x1 && this->y1 == s.y1 &&
			this->x2 == s.x2 && this->y2 == s.y2)
		{
			return false;
		}
		else {
			return true;
		}
	}
};

void ed_printSurface(ed_Surface& s);

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
class ed_SceneEditor;

class ed_RenderObject {
private:
	int distWX = 0, distWY = 0;
	int distTX = 0, distTY = 0;
protected:
	std::vector<std::vector<SDL_Texture*>> sheets; //spritesheet

	//corresponds to spritesheet
	std::vector<std::vector<ed_Surface>> collisionGroups;
	std::vector<std::vector<SDL_Rect>> renderGroups;

	SDL_Texture* currentTexture;

	void updateToCurrentTexture(size_t _sheetIndex, size_t _textureIndex)
	{
		ed_Surface* currentSurface = &this->collisionGroups[this->sheetIndex][this->textureIndex];
		SDL_Rect* currentRect = &this->renderGroups[this->sheetIndex][this->textureIndex];

		ed_Surface* updatedSurface = &this->collisionGroups[_sheetIndex][_textureIndex];
		SDL_Rect* updatedRect = &this->renderGroups[_sheetIndex][_textureIndex];
		
		//collision constant's distance from center x and y
		int distanceFromSX = updatedSurface->centerX - updatedSurface->x1;
		int distanceFromSY = updatedSurface->centerY - updatedSurface->y1;

		//set centers to be the same
		updatedSurface->centerX = currentSurface->centerX;
		updatedSurface->centerY = currentSurface->centerY;

		updatedSurface->x1 = updatedSurface->centerX - distanceFromSX;
		updatedSurface->x2 = updatedSurface->centerX + distanceFromSX;
		updatedSurface->y1 = updatedSurface->centerY - distanceFromSY;
		updatedSurface->y2 = updatedSurface->centerY + distanceFromSY;

		updatedRect->x = currentRect->x;
		updatedRect->y = currentRect->y;
	}

	void updateToCurrentOutsideTexture(size_t _sheetIndex, size_t _textureIndex)
	{
		ed_Surface* currentSurface = &this->collisionGroups[this->sheetIndex][this->textureIndex];
		SDL_Rect* currentRect = &this->renderGroups[this->sheetIndex][this->textureIndex];

		ed_Surface* updatedSurface = &this->collisionGroups[_sheetIndex][_textureIndex];
		SDL_Rect* updatedRect = &this->renderGroups[_sheetIndex][_textureIndex];
	}

	size_t sheetIndex = 0;
	size_t textureIndex = 0;

	size_t deltaTexture = 1;

	int deltaCamX = 0, deltaCamY = 0;
	int deltaWorldX = 0, deltaWorldY = 0;

	const int distFromCX = 0, distFromCY = 0;
public:
	std::vector<void(*)()> backgroundMethods;

	ed_Surface surfaceBelow;

	void scale(int windowWidth, int windowHeight)
	{
		auto scaledRelativePosition = [](int p1, int m1, int m2) {
			int relativePosition = round(1.0 * p1 / m1);

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

	int getRX()
	{
		return renderGroups[sheetIndex][textureIndex].x;
	}

	int getRY()
	{
		return renderGroups[sheetIndex][textureIndex].y;
	}

	int getX1()
	{
		return getCollisionBox().x1;
	}

	int getY1()
	{
		return getCollisionBox().y1;
	}

	int getX2()
	{
		return getCollisionBox().x2;
	}

	int getY2() 
	{
		return getCollisionBox().y2;
	}

	int getCX()
	{
		return getCollisionBox().centerX;
	} 

	int getCY()
	{
		return getCollisionBox().centerY;
	}

	void setPos(int x, int y) {
		int distX = x - getCollisionBox().centerX;
		int distY = y - getCollisionBox().centerY;

		for (std::vector<ed_Surface>& sv : collisionGroups) {
			for (ed_Surface& s : sv) {
				s.x1 += distX;
				s.x2 += distX;
				s.centerX += distX;

				s.y1 += distY;
				s.y2 += distY;
				s.centerY += distY;
			}
		}

		for (std::vector<SDL_Rect>& tv : renderGroups) {
			for (SDL_Rect& r : tv) {
				r.x += distX;
				r.y += distY;
			}
		}
	}

	void setRenderPos(int x, int y)
	{
		SDL_Rect* rect = &this->renderGroups[this->getSheetIndex()][this->getTextureIndex()];

		int distX = x - rect->x;
		int distY = y - rect->y;

		for (size_t i = 0; i < renderGroups.size(); i++) {
			for (size_t j = 0; j < renderGroups.size(); j++) {
				renderGroups[i][j].x += distX;
				renderGroups[i][j].y += distY;
			}
		}

		for (std::vector<ed_Surface>& sv : collisionGroups) {
			for (ed_Surface& s : sv) {
				s.x1 += distX;
				s.x2 += distX;
				s.centerX += distX;

				s.y1 += distY;
				s.y2 += distY;
				s.centerY += distY;
			}
		}
	}

	void changeTexture(size_t sheetIndex, size_t textureIndex) 
	{
		this->sheetIndex = sheetIndex;
		this->textureIndex = textureIndex;

		this->currentTexture = this->sheets[sheetIndex][textureIndex];
	}

	size_t getTextureGroupSize(int sheetIndex)
	{
		return this->sheets[sheetIndex].size();
	}

	std::vector<std::vector<ed_Surface>> getCollisionGroups()
	{
		return collisionGroups;
	}

	void worldMove(ed_Dir xDir, ed_Dir yDir)
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

		for (size_t i = 0; i < collisionGroups.size(); i++) {
			for (size_t j = 0; j < collisionGroups[i].size(); j++) {
				collisionGroups[i][j].x1 += getDeltaWorldX() * xWay;
				collisionGroups[i][j].x2 += getDeltaWorldX() * xWay;
				collisionGroups[i][j].centerX += getDeltaWorldX() * xWay;

				collisionGroups[i][j].y1 += getDeltaWorldX() * yWay;
				collisionGroups[i][j].y2 += getDeltaWorldX() * yWay;
				collisionGroups[i][j].centerY += getDeltaWorldX() * yWay;
			}
		}
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

		//test
		for (size_t i = 0; i < renderGroups.size(); i++) {
			for (size_t j = 0; j < renderGroups[i].size(); j++) {
				renderGroups[i][j].x += getDeltaCamX() * xWay;
				renderGroups[i][j].y += getDeltaCamX() * yWay;
			}
		}
	}

	bool falling = false;

	bool xCamLocked = false, yCamLocked = false;

	friend class ed_Scene;
	friend class ed_RenderObjectCreator;
	friend class ed_SceneEditor;

	Uint32 ID = 0;
};

class ed_AnimateObject : public ed_RenderObject {
protected:
	virtual int typeIndex() = 0; //where type base object is stored in renderObject pointer group
public:
	bool traveling = false;

	bool moving = false;

	int getTypeIndex()
	{
		return typeIndex();
	}
};

class ed_Player : public ed_AnimateObject {
protected:
	int typeIndex() override {
		return PLAYER_TYPE;
	}
public:
	virtual void jump();

	int health = 0;
};

#endif