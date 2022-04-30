#include "scene_editor.h"

void renderCollisionBox(ed_Surface s) {
	SDL_Rect collisionRect = { s.x1, s.y1, s.x2 - s.x1, s.y2 - s.y1 };

	SDL_RenderDrawRect(ed_mainRenderer, &collisionRect);
}

void ed_SceneEditor::editScene()
{
	ed_running = true;

	const int fps = 60;
	Uint32 waitTime, endTime;

	scene->setTexturePositions();

	while (ed_running) {
		while (SDL_PollEvent(&ed_input)) {
			switch (ed_input.type) {
			case SDL_QUIT:
				ed_running = false;
				break;
			}
		}

		waitTime = 1000 / fps;
		endTime = SDL_GetTicks() + waitTime;

		SDL_RenderClear(ed_mainRenderer);

		//render backgrounds
		for (ed_RenderObject& background : scene->backgrounds) {
			SDL_RenderCopy(ed_mainRenderer, background.getCurrentTexture(), NULL, background.getTextureBox());
		}

		//render foreground objects
		for (ed_RenderObject& texture : scene->foregroundObjects) {
			SDL_RenderCopy(ed_mainRenderer, texture.getCurrentTexture(), NULL, texture.getTextureBox());
		}

		for (ed_RenderObject *r : allRenderObjects) {
			renderCollisionBox(r->getCollisionBox());
		}

		for (ed_Surface& s : scene->surfaces) {
			renderCollisionBox(s);
		}

		scene->render();

		if (SDL_GetTicks() < endTime) {
			SDL_Delay(endTime - SDL_GetTicks());

			SDL_RenderPresent(ed_mainRenderer);
		} else {
			SDL_RenderPresent(ed_mainRenderer);
		}
	}
}