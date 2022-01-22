#include "player.h"

void moveHorizontally(int constMovementInterval, int scanKeyID, int direction, const Uint8* key, Uint32 spriteChangeInterval, int sheetIndex) 
{
	ed_Player.tex.sheetIndex = 1;
	ed_Player.tex.textureIndex = 1;

	size_t lastSpriteIndex = ed_Player.tex.sheets[0].size() - 1;

	ed_Player.tex.sheetIndex = sheetIndex;

	while (true) {
		if (!key[scanKeyID]) {
			break;
		}

		if (SDL_GetTicks() >= spriteChangeInterval) {
			spriteChangeInterval += constMovementInterval;

			if (ed_Player.tex.textureIndex == lastSpriteIndex) {
				ed_Player.tex.textureIndex = 1;
			}
			else {
				ed_Player.tex.textureIndex++;
			}
		}

		ed_Player.tex.ren.x += ed_Player.tex.deltaCamX * direction;
		ed_Player.tex.world.x += ed_Player.tex.deltaWorldX * direction;

		spriteChangeInterval -= 20;

		SDL_Delay(20);
	}
}

void ed_pMovementInput(int constMovementInterval) 
{
	SDL_Event evt;

	const Uint8* key = SDL_GetKeyboardState(NULL);

	Uint32 spriteChangeInterval = SDL_GetTicks() + constMovementInterval;

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

			ed_Player.tex.textureIndex = 0;
		} 
	}
}


