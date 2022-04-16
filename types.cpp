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

			this->worldMove(ed_Dir::NONE, ed_Dir::UP);

			if (!this->yCamLocked) {
				this->camMove(ed_Dir::NONE, ed_Dir::UP);
			}
		}
	}
}

