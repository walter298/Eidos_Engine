#include "player_movement.h"

void moveHorizontally(int constMovementInterval, int scanKeyID, ed_Dir direction, const Uint8* key, Uint32 spriteChangeInterval, int sheetIndex) 
{
	c_Player.changeTexture(sheetIndex, 1);

	size_t lastSpriteIndex = c_Player.getTextureGroupSize(sheetIndex) - 1;

	c_Player.changeTexture(sheetIndex, c_Player.getTextureIndex());

	while (true) {
		if (!key[scanKeyID]) {
			break;
		}

		if (SDL_GetTicks() >= spriteChangeInterval) {
			spriteChangeInterval += constMovementInterval;

			if (c_Player.getTextureIndex() == lastSpriteIndex) 
			{
				c_Player.changeTexture(c_Player.getSheetIndex(), 1);
			} else {
				c_Player.changeTexture(c_Player.getSheetIndex(), c_Player.getTextureIndex() + 1);
			}
		}

		if (!c_Player.xCamLocked) {
			c_Player.camMove(direction, ed_Dir::NONE);
		}

		c_Player.worldMove(direction, ed_Dir::NONE);

		spriteChangeInterval -= 20;

		SDL_Delay(20);
	}
}

void ed_jumpInput(int constMovementInterval)
{
	SDL_Event evt;

	const Uint8* key = SDL_GetKeyboardState(NULL);

	Uint32 jumpInterval = SDL_GetTicks() + constMovementInterval;

	int jumpHeight;

	while (ed_running) {
		if (key[SDL_SCANCODE_SPACE] && !c_Player.falling) {
			c_Player.jump();
		}

		SDL_Delay(5);

		while (c_Player.falling) {} //wait
	}
}

void ed_pMovementInput(int constMovementInterval) 
{
	SDL_Event evt;

	const Uint8* key = SDL_GetKeyboardState(NULL);

	Uint32 spriteChangeInterval = SDL_GetTicks() + constMovementInterval;

	std::thread jumpThread(ed_jumpInput, 5);
	jumpThread.detach();

	while (ed_running) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				Uint32 spriteChangeInterval = SDL_GetTicks() + constMovementInterval;

				if (key[SDL_SCANCODE_D]) {
					moveHorizontally(constMovementInterval, 7, ed_Dir::RIGHT, key, spriteChangeInterval, 1);
				} else if (key[SDL_SCANCODE_S]) {
					moveHorizontally(constMovementInterval, 22, ed_Dir::LEFT, key, spriteChangeInterval, 0);
				}
			}

			c_Player.changeTexture(c_Player.getSheetIndex(), 0);
		} 
	}
}