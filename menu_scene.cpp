#include "menu_scene.h"

void ed_checkButtonInputs()
{
	SDL_Event evt;

	std::cout << "about to run\n";

	while (ed_running) {
		//std::cout << "running\n";
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_MOUSEBUTTONDOWN) {
				std::cout << "mouse button pressed\n";

				for (ed_Button& button : ed_globalScene->buttons) {
					if (button.hovered()) {
						std::cout << "executing reaction\n";
						button.reaction();
					}
				}
			}
		}
	}
}

void ed_Menu::init()
{
	std::cout << "initializing\n";

	ed_runningThreads.insert(ed_runningThreads.begin(), std::thread(ed_checkButtonInputs));
}