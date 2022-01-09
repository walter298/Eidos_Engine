#include "player.h"

void ed_pMovementInput(int movementInterval)
{
	SDL_Event evt;

	const Uint8* key = SDL_GetKeyboardState(NULL);

	Uint32 nextMovementInterval;

	while (ed_globalScene.executing) {

		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				std::cout << ed_globalScene.foregroundObjects[0].deltaCamX << ", " << ed_globalScene.foregroundObjects[0].deltaWorldX << std::endl;

				if (key[SDL_SCANCODE_D]) {
					nextMovementInterval = SDL_GetTicks() + movementInterval;

					while (true) {
						if (!key[SDL_SCANCODE_D]) {
							break;
						}

						if (SDL_GetTicks() >= nextMovementInterval) {
							ed_globalScene.foregroundObjects[0].ren.x += ed_globalScene.foregroundObjects[0].deltaCamX;
							ed_globalScene.foregroundObjects[0].world.x += ed_globalScene.foregroundObjects[0].deltaWorldX;

							nextMovementInterval += movementInterval;
						}
					}
				}
				else if (key[SDL_SCANCODE_S]) {
					nextMovementInterval = SDL_GetTicks() + movementInterval;

					while (true) {
						if (!key[SDL_SCANCODE_S]) {
							break;
						}

						if (SDL_GetTicks() >= nextMovementInterval) {
							ed_globalScene.foregroundObjects[0].ren.x -= ed_globalScene.foregroundObjects[0].deltaCamX;
							ed_globalScene.foregroundObjects[0].world.x -= ed_globalScene.foregroundObjects[0].deltaWorldX;

							nextMovementInterval += movementInterval;
						}
					}
				}
			}
		}
	}
}


