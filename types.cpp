#include "types.h"

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

void ed_Character::updateTexture(int sheetIndex, int textureIndex)
{
	auto *tex = &this->tex;

	//our current render position
	int CRX = tex->renderGroups[tex->sheetIndex][tex->textureIndex].x;
	int CRY = tex->renderGroups[tex->sheetIndex][tex->textureIndex].y;

	//set new texture to be at current position
	tex->renderGroups[sheetIndex][textureIndex].x = CRX;
	tex->renderGroups[sheetIndex][textureIndex].y = CRY;

	//our current collision position
	/*int CRX1 = this->tex.collisionGroups[sheetIndex][spriteIndex].x1;
	int CRY1 = this->tex.collisionGroups[sheetIndex][spriteIndex].y1;
	int CRX2 = this->tex.collisionGroups[sheetIndex][spriteIndex].x2;
	int CRY2 = this->tex.collisionGroups[sheetIndex][spriteIndex].y2;*/
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