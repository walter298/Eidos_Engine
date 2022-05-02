#include "scene_editor.h"

bool doneWithInputThread;

bool selectingSurface = false;

ed_Surface* editedRect;
ed_RenderObject* obj;

void ed_SceneEditor::save()
{
	std::cout << "Saving...\n";

	std::vector<std::vector<std::string>> unchangedData, newData;

	std::string line;

	std::fstream levelFile;
	levelFile.open(path);

	int dataCount = -1;

	bool writingUnchangedData = false;

	//get unchanged data
	while (std::getline(levelFile, line)) {
		if (line.empty()) {
			continue;
		}

		if (line == "end") {
			writingUnchangedData = false;

			continue;
		}

		if (writingUnchangedData) {
			unchangedData[dataCount].push_back(line);

			continue;
		}

		if (line == "BACKGROUND_IMAGES") {
			writingUnchangedData = true;

			dataCount++;
			unchangedData.push_back({});

			unchangedData[dataCount].push_back(line);

			continue;
		}
	}

	levelFile.close();
	std::remove(path.c_str());

	int nDataCount = 0;

	if (hasPlayer) {

		newData.push_back({});

		newData[nDataCount].push_back("PLAYER_PLACEMENT");

		std::string placement = ed_writeNums({ c_Player.getCollisionBox().centerX, c_Player.getCollisionBox().centerY });
		newData[nDataCount].push_back(placement);

		nDataCount++;
	}

	if (scene->surfaces.size() > 0) {
		newData.push_back({});

		for (ed_Surface& s : scene->surfaces) {
			newData[nDataCount].push_back("FLOOR");

			std::string line1 = ed_writeNums({ s.x1, s.x2 });
			std::string line2 = ed_writeNums({ s.y1, s.y2 });

			newData[nDataCount].push_back(line1);
			newData[nDataCount].push_back(line2);
		}
	}

	std::ofstream newFile(path);

	std::cout << "Carried over data:\n";
	for (std::vector<std::string> &sv : unchangedData) {

		newFile << std::endl;
		for (std::string& s : sv) {
			std::cout << "writing old data " << s << std::endl;
			newFile << s << std::endl;
		}

		newFile << "end\n";
	}

	for (std::vector<std::string>& sv : newData) {
		newFile << std::endl;
		for (std::string& s : sv) {
			newFile << s << std::endl;
		}

		newFile << "end\n";
	}

	newFile.close();
}

void ed_SceneEditor::getSelectedSurface()
{
	size_t rectIndex = 0;

	while (!doneWithInputThread) {
		if (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				if (ed_keyState[SDL_SCANCODE_UP]) {
					if (rectIndex + 1 == scene->surfaces.size()) {
						rectIndex = 0;
					}
					else {
						rectIndex++;
					}

					editedRect = &scene->surfaces[rectIndex];
				}
			}
		}
	}
}

void moveRectHr(ed_Surface* rect) {
	rect->x1 += 1;
	rect->x2 += 1;
	rect->centerX += 1;
}

void moveRectVr(ed_Surface* rect) {
	rect->y1 += 1;
	rect->y2 += 1;
	rect->centerY += 1;
}

void ed_SceneEditor::editSurface()
{
	editedRect = &scene->surfaces[0];

	selectingSurface = true;

	std::thread selectedSurface = std::thread(&ed_SceneEditor::getSelectedSurface, this);
	selectedSurface.detach();

	while (!doneWithInputThread) {
		if (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				if (ed_keyState[SDL_SCANCODE_W]) {
					moveRectVr(editedRect);

					SDL_Delay(10);
				}
			}
		}
 	}

	selectingSurface = false;
}

void ed_SceneEditor::editObject()
{
	obj = allRenderObjects[0];

	std::thread t(&ed_SceneEditor::getSelectedObject, this);
	t.detach();

	size_t objectIndex = 0;

	while (!doneWithInputThread) {
		while (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				if (ed_keyState[SDL_SCANCODE_UP]) {
					if (objectIndex + 1 == allRenderObjects.size()) {
						objectIndex = 0;
					} else {
						objectIndex++;
					}

					obj = allRenderObjects[objectIndex];

					SDL_Delay(5);
				}
			}
		}
	}
}

void ed_SceneEditor::getSelectedObject()
{
	while (!doneWithInputThread) {
		if (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				obj->setPos(x, y);
			}
		}
	}
}

void ed_SceneEditor::getInput()
{
	std::thread currentThread;

	std::string command;

	while (ed_running) {
		std::cin >> command;

		doneWithInputThread = false;

		if (command == "edit_surface") {
			currentThread = std::thread(&ed_SceneEditor::editSurface, this);
			currentThread.detach();
		} else if (command == "move_object") {
			currentThread = std::thread(&ed_SceneEditor::editObject, this);
			currentThread.detach();
		} else if (command == "save_exit") {
			save();
			ed_running = false;
		} else {
			std::cout << "Error: unkown command: " << command << std::endl;
		}
	}
}

void renderCollisionBox(ed_Surface* s) {
	SDL_Rect collisionRect = { s->x1, s->y1, s->x2 - s->x1, s->y2 - s->y1 };

	SDL_RenderDrawRect(ed_mainRenderer, &collisionRect);
}

void ed_SceneEditor::editScene()
{
	ed_running = true;

	const int fps = 60;
	Uint32 waitTime, endTime;

	scene->setTexturePositions();

	std::thread inputThread(&ed_SceneEditor::getInput, this);
	inputThread.detach();

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
			//renderCollisionBox(&r->getCollisionBox());
		}

		if (selectingSurface) {
			renderCollisionBox(editedRect);
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