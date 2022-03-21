#include "player_movement.h"

void moveHorizontally(int constMovementInterval, int scanKeyID, int direction, const Uint8* key, Uint32 spriteChangeInterval, int sheetIndex) 
{
	c_Player.tex.updateToCurrentTexture(sheetIndex, 1);

	c_Player.tex.sheetIndex = 1;
	c_Player.tex.textureIndex = 1;

	size_t lastSpriteIndex = c_Player.tex.sheets[0].size() - 1;

	c_Player.tex.sheetIndex = sheetIndex;

	while (true) {
		if (!key[scanKeyID]) {
			break;
		}

		if (SDL_GetTicks() >= spriteChangeInterval) {
			spriteChangeInterval += constMovementInterval;

			if (c_Player.tex.textureIndex == lastSpriteIndex) 
			{
				c_Player.tex.updateToCurrentTexture(c_Player.tex.sheetIndex, 1);
				c_Player.tex.textureIndex = 1;
			} else 
			{
				c_Player.tex.updateToCurrentTexture(c_Player.tex.sheetIndex, c_Player.tex.textureIndex + 1);

				c_Player.tex.textureIndex++;
			}
		}

		c_Player.tex.renderGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x += c_Player.tex.deltaCamX * direction;

		c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x1 += c_Player.tex.deltaWorldX * direction;
		c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x2 += c_Player.tex.deltaWorldX * direction;

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

	while (ed_globalScene.executing) {
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

	while (ed_globalScene.executing) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				Uint32 spriteChangeInterval = SDL_GetTicks() + constMovementInterval;

				if (key[SDL_SCANCODE_D]) {
					moveHorizontally(constMovementInterval, 7, 1, key, spriteChangeInterval, 1);
				} else if (key[SDL_SCANCODE_S]) {
					moveHorizontally(constMovementInterval, 22, -1, key, spriteChangeInterval, 0);
				}
			}

			c_Player.tex.updateToCurrentTexture(c_Player.tex.sheetIndex, 0);
			c_Player.tex.textureIndex = 0;
		} 
	}
}