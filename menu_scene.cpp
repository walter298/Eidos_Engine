#include "menu_scene.h"

void ed_checkButtonInputs()
{
	SDL_Event evt;

	while (ed_running) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_MOUSEBUTTONDOWN) {
				for (ed_Button& button : ed_globalScene->buttons) {
					if (button.hovered()) {
						button.reaction();
					}
				}
			}
		}
	}
}

void ed_Menu::init()
{
	ed_runningThreads.insert(ed_runningThreads.begin(), std::thread(ed_checkButtonInputs));
}