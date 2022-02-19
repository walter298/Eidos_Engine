#include "scene.h"

void ed_checkButtonInputs()
{
	SDL_Event evt;

	while (ed_globalScene.executing) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_MOUSEBUTTONDOWN) {
				for (ed_Button& button : ed_globalScene.buttons) {
					if (button.hovered()) {
						button.reaction();
					}
				}
			}
		}
	}
}

std::vector<std::thread> threads;

void ed_executeScene(ed_Scene scene)
{
	ed_globalScene = scene;

	for (void(*method)() : ed_globalScene.backgroundMethods) {
		threads.insert(threads.begin(), (std::thread(method)));
		threads[0].detach();
	}

	ed_globalScene.executing = true;

	if (ed_globalScene.containsPlayer) {
		threads.insert(threads.begin(), std::thread(ed_pMovementInput, 70));
		threads[0].detach();

		/*threads.insert(threads.begin(), std::thread(ed_checkPlayerCollision));
		threads[0].detach();*/
	}

	if (ed_globalScene.buttons.size() > 0) {
		threads.insert(threads.begin(), std::thread(ed_checkButtonInputs));
	}

	SDL_Event evt;

	const Uint8* mainKey = SDL_GetKeyboardState(NULL); //handles main inputs like quitting the game

	Uint32 fps = 60;
	Uint32 startingTick;

	while (ed_globalScene.executing) {
		startingTick = SDL_GetTicks();

		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				ed_globalScene.executing = false;
				break;
			case SDL_KEYDOWN:
				if (mainKey[SDL_SCANCODE_O]) {
					scene.executing = false;
				}
				break;
			}
		}

		SDL_RenderClear(ed_mainRenderer);

		//render backgrounds
		for (ed_Texture& background : ed_globalScene.backgrounds) {
			SDL_RenderCopy(ed_mainRenderer, background.sheets[0][0], NULL, &background.renderGroups[0][0]);
		}

		//render foreground objects
		for (ed_Texture& texture : ed_globalScene.foregroundObjects) {
			SDL_RenderCopy(ed_mainRenderer, texture.sheets[texture.sheetIndex][texture.textureIndex], NULL, 
				&texture.renderGroups[texture.sheetIndex][texture.textureIndex]);
		}

		//render player
		if (ed_globalScene.containsPlayer) {
			SDL_RenderCopy(ed_mainRenderer, c_Player.tex.sheets[c_Player.tex.sheetIndex][c_Player.tex.textureIndex], NULL,
				&c_Player.tex.renderGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex]);
		}

		SDL_RenderPresent(ed_mainRenderer);

		SDL_Delay(10);
	}
}