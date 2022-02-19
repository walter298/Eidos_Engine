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