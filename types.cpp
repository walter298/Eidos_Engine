#include "types.h"

void ed_Surface::init(int x1, int y1, int x2, int y2, int cX, int cY)
{
	this->x1 = x1, this->y1 = y1;
	this->x2 = x2, this->y2 = y2;
	this->centerX = cX, this->centerY = cY;

	this->x = x1, this->y = y1;
	this->w = this->x2 - this->x1;
	this->h = this->y2 - this->y1;
}

bool ed_Button::hovered()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX > this->x1 && mouseX < this->x2 &&
		mouseY > this->y1 && mouseY < this->y2) 
	{
		return true;
	}

	return false;
}

void ed_Texture::updateToCurrentTexture(int sheetIndex, int textureIndex)
{
	//our current render position
	int CRX = this->renderGroups[this->sheetIndex][this->textureIndex].x;
	int CRY = this->renderGroups[this->sheetIndex][this->textureIndex].y;

	//set new texture to be at current position
	this->renderGroups[sheetIndex][textureIndex].x = CRX;
	this->renderGroups[sheetIndex][textureIndex].y = CRY;
	//our current collision position
	/*int CRX1 = this->tex.collisionGroups[sheetIndex][spriteIndex].x1;
	int CRY1 = this->tex.collisionGroups[sheetIndex][spriteIndex].y1;
	int CRX2 = this->tex.collisionGroups[sheetIndex][spriteIndex].x2;
	int CRY2 = this->tex.collisionGroups[sheetIndex][spriteIndex].y2;*/
}

void ed_Texture::updateToPosition(int sheetIndex, int textureIndex, int x, int y)
{
	auto* collisionBox = &this->collisionGroups[sheetIndex][textureIndex];
	auto* texture = &this->renderGroups[sheetIndex][textureIndex];

	std::cout << "original coords: " << collisionBox->x1 << ", " << collisionBox->y1 << std::endl;

	int xDistance = x - collisionBox->centerX;
	int yDistance = y - collisionBox->centerY;

	collisionBox->centerX += xDistance;
	collisionBox->centerY += yDistance;
	collisionBox->x1 += xDistance;
	collisionBox->y1 += yDistance;
	collisionBox->x2 += xDistance;
	collisionBox->y2 += yDistance;

	std::cout << "new coords: " << collisionBox->x1 << ", " << collisionBox->y1 << std::endl;

	texture->x += xDistance;
	texture->y += yDistance;
}

void ed_Player::jump() {
	int jumpTime = SDL_GetTicks() + 100;

	int jumpInterval = SDL_GetTicks() + 5;

	this->jumping = true;

	while (true) {
		if (SDL_GetTicks() == jumpTime) {
			this->jumping = false;

			break;
		}

		if (SDL_GetTicks() == jumpInterval) {
			jumpInterval += 5;

			auto c = &this->tex;

			c->collisionGroups[c->sheetIndex][c->textureIndex].y1 -= this->tex.deltaWorldY;
			c->collisionGroups[c->sheetIndex][c->textureIndex].y2 -= this->tex.deltaWorldY;

			c->renderGroups[c->sheetIndex][c->textureIndex].y -= this->tex.deltaCamY;
		}
	}
}