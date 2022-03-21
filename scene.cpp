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

ed_Surface c;

SDL_Rect rect;

std::vector<std::thread> threads;

void ed_executeScene(ed_Scene scene)
{
	ed_globalScene = scene;

	const int fps = 60;

	for (void(*method)() : ed_globalScene.backgroundMethods) {
		threads.insert(threads.begin(), (std::thread(method)));
		threads[0].detach();
	}

	ed_globalScene.executing = true;

	if (ed_globalScene.containsPlayer) {
		threads.insert(threads.begin(), std::thread(ed_pMovementInput, 70));
		threads[0].detach();

		threads.insert(threads.begin(), std::thread(ed_checkPlayerCollision));
		threads[0].detach();
	}

	if (ed_globalScene.buttons.size() > 0) {
		threads.insert(threads.begin(), std::thread(ed_checkButtonInputs));
	}

	SDL_Event evt;

	const Uint8* mainKey = SDL_GetKeyboardState(NULL); //handles main inputs like quitting the game

	c = c_Player.tex.collisionGroups[0][0];
	rect = { c.x1, c.y1, c.x2 - c.x1, c.y2 - c.y1 };

	std::cout << "new rect coords: \n";
	std::cout << rect.x << ", " << rect.y << std::endl;

	while (ed_globalScene.executing) {
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

		int waitTime = 1000 / fps;
		int endTime = SDL_GetTicks() + waitTime;

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
			

			SDL_SetRenderDrawColor(ed_mainRenderer, 0xFF, 0, 0, 0xFF);
			SDL_RenderDrawRect(ed_mainRenderer, &c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex]);
		}

		while (SDL_GetTicks() + (1000 / fps) < endTime) {}

		SDL_RenderPresent(ed_mainRenderer);
	}
}