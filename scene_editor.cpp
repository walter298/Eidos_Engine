#include "scene_editor.h"

bool doneWithInputThread;

bool selectingSurface = false;

ed_Surface* editedSurface;
ed_RenderObject* obj;

int deltaCamX = 0, deltaCamY = 0;

void ed_SceneEditor::save()
{
	std::cout << "Saving...\n";

	std::vector<std::string> backgroundImageData;
	std::vector<std::vector<std::string>> newData;

	std::string line;

	std::fstream levelFile;
	levelFile.open(path);

	int dataCount = -1;

	bool writingBackgroundImageData = false;

	//get background image data
	while (std::getline(levelFile, line)) {
		if (line.empty()) { //ignore empty lines
			continue;
		}

		if (line == "end") {
			writingBackgroundImageData = false;

			break;
		}

		if (writingBackgroundImageData) {
			backgroundImageData.push_back(line);

			continue;
		}

		if (line == "BACKGROUND_IMAGES") {
			writingBackgroundImageData = true;

			dataCount++;

			backgroundImageData.push_back(line);

			continue;
		}
	}

	backgroundImageData.push_back("end");

	levelFile.close();
	std::remove(path.c_str());

	int nDataCount = 0;

	if (hasPlayer) {
		newData.push_back({});

		newData[nDataCount].push_back("PLAYER_PLACEMENT");

		std::string placement = ed_writeNums({ c_Player.getCollisionBox().centerX, c_Player.getCollisionBox().centerY });
		newData[nDataCount].push_back(placement);

		newData[nDataCount].push_back("end");

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

			newData[nDataCount].push_back("end");
		}

		nDataCount++;
	}

	if (scene->camLocks.size() > 0) {
		newData.push_back({});

		for (ed_CamLockState& c : scene->camLocks) {
			newData[nDataCount].push_back("CAMERA_LOCK");

			std::string nums = ed_writeNums({c.getX1(), c.getY1(), c.getX2(), c.getY2(), c.getXLocked(), c.getYLocked()});
			newData[nDataCount].push_back(nums);

			newData[nDataCount].push_back("end");
		}
	}

	std::ofstream newFile(path);

	//write old data
	for (std::string &s : backgroundImageData) {
		newFile << s << std::endl;
	}

	for (std::vector<std::string>& sv : newData) {
		newFile << std::endl;
		for (std::string& s : sv) {
			newFile << s << std::endl;
		}
	}

	newFile.close();
}

void ed_SceneEditor::getSelectedSurface()
{
	doneWithInputThread = false;

	selectingSurface = true;

	size_t rectIndex = 0;

	while (!doneWithInputThread) {
		while (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				if (ed_keyState[SDL_SCANCODE_C]) {
					if (rectIndex + 1 == scene->surfaces.size()) {
						rectIndex = 0;
					} else {
						rectIndex++;
					}

					std::cout << "selected " << rectIndex << "th surface\n";

					editedSurface = &scene->surfaces[rectIndex];
				}
			}
		}
	}

	selectingSurface = false;
}

void moveRectHr(ed_Surface* rect, int dir) {
	rect->x1 += 4 * dir;
	rect->x2 += 4 * dir;
	rect->centerX += 4 * dir;
}

void moveRectVr(ed_Surface* rect, int dir) {
	rect->y1 += 4 * dir;
	rect->y2 += 4 * dir;
	rect->centerY += 4 * dir;
}

void changeSurfaceSize(ed_Surface* s, int deltaX, int deltaY)
{
	s->x2 += deltaX;
	s->y2 += deltaY;
}

void ed_SceneEditor::editSurface()
{
	doneWithInputThread = false;

	editedSurface = &scene->surfaces[0];

	selectingSurface = true;

	std::thread selectedSurface = std::thread(&ed_SceneEditor::getSelectedSurface, this);
	selectedSurface.detach();

	while (!doneWithInputThread) {
		while (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				if (ed_keyState[SDL_SCANCODE_S]) {
					moveRectVr(editedSurface, 1);
				} else if (ed_keyState[SDL_SCANCODE_W]) {
					moveRectVr(editedSurface, -1);
				} else if (ed_keyState[SDL_SCANCODE_A]) {
					moveRectHr(editedSurface, -1);
				} else if (ed_keyState[SDL_SCANCODE_D]) {
					moveRectHr(editedSurface, 1);
				} else if (ed_keyState[SDL_SCANCODE_RIGHT]) {
					changeSurfaceSize(editedSurface, 4, 0);
				} else if (ed_keyState[SDL_SCANCODE_LEFT]) {
					changeSurfaceSize(editedSurface, -4, 0);
				} else if (ed_keyState[SDL_SCANCODE_DOWN]) {
					changeSurfaceSize(editedSurface, 0, 4);
				} else if (ed_keyState[SDL_SCANCODE_UP]) {
					changeSurfaceSize(editedSurface, 0, -4);
				}
			}
		}
 	}
}

void ed_SceneEditor::getSelectedObject()
{
	doneWithInputThread = false;

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

					SDL_Delay(1);
				}
			}
		}
	}
}

void ed_SceneEditor::editObject()
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

void ed_SceneEditor::checkCamMovement()
{
	doneWithInputThread = false;

	while (!doneWithInputThread) {
		if (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_KEYDOWN) {
				int dCX = 0, dCY = 0;

				int keyPressed = 0, deltaX = 0, deltaY = 0;

				if (ed_keyState[ED_ARROW_RIGHT]) {
					keyPressed = ED_ARROW_RIGHT;

					deltaX = -10;
					dCX = 10;
				} else if (ed_keyState[ED_ARROW_LEFT]) {
					keyPressed = ED_ARROW_LEFT;
					deltaX = 10;
					dCX = -10;
				} else if (ed_keyState[ED_ARROW_DOWN]) {
					keyPressed = ED_ARROW_DOWN;
					deltaY = -10;
					dCY = 10;
				} else if (ed_keyState[ED_ARROW_UP]) {
					keyPressed = ED_ARROW_UP;
					deltaY = 10;
					dCY = -10;
				} else {
					std::cout << "Error: invalid key is pressed\n";

					return;
				}

				while (true) {
					if (!ed_keyState[keyPressed]) {
						break;
					}

					deltaCamX += dCX;
					deltaCamY += dCY;

					std::cout << "deltaCamX: " << deltaCamX << std::endl;
					std::cout << "deltaCamY: " << deltaCamY << std::endl;

					scene->moveCam(deltaX, deltaY);
					

					SDL_Delay(10);
				}
			}
		}
	}
}

void getMouseXY(int& x, int& y)
{
	
	while (true) {
		if (SDL_PollEvent(&ed_input)) {
			if (ed_input.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&x, &y);
				std::cout << "mouse click: " << x << ", " << y << std::endl;
				
				break;
			}
		}
	}

}

void ed_SceneEditor::drawRect(int& x1, int& y1, int& x2, int& y2)
{
	bool sec = false;

	std::cout << "click on x1 and y1\n";

	getMouseXY(x1, y1);
	getMouseXY(x2, y2);

	std::cout << "injected surface\n";

	std::cout << "deltas\n";
	printTwo(deltaCamX, deltaCamY);

	//adjust for camera moving
	x1 += deltaCamX;
	x2 += deltaCamX;
	y1 += deltaCamY;
	y2 += deltaCamY;
}

void ed_SceneEditor::injectSurface()
{
	int x1, y1, x2, y2;

	drawRect(x1, y1, x2, y2);

	std::cout << "New Surface: \n";
	printTwo(x1, y1);
	printTwo(x2, y2);

	scene->surfaces.push_back({x1, y1, x2, y2});
}

void ed_SceneEditor::injectCamLockState()
{
	int x1, y1, x2, y2;

	drawRect(x1, y1, x2, y2);

	bool x, y;

	std::cout << "enter x locked boolean\n";
	std::cin >> x;

	std::cout << "enter y locked boolean\n";
	std::cin >> y;

	ed_CamLockState state(x1, y1, x2, y2, x, y);

	scene->camLocks.push_back(state);
}

void ed_SceneEditor::getInput()
{
	std::thread currentThread;

	std::string command;

	while (ed_running) {
		std::cin >> command;

		doneWithInputThread = true;

		if (command == "edit_surface") {
			currentThread = std::thread(&ed_SceneEditor::editSurface, this);
			currentThread.detach();
		} else if (command == "move_object") {
			currentThread = std::thread(&ed_SceneEditor::editObject, this);
			currentThread.detach();
		}
		else if (command == "move_cam") {
			currentThread = std::thread(&ed_SceneEditor::checkCamMovement, this);
			currentThread.detach();
		}
		else if (command == "inject_surface") {
			this->injectSurface();
		}
		else if (command == "add_camlock") {
			injectCamLockState();
		} else if (command == "add_render_object") {

		} else if (command == "save_exit") {
			save();
			ed_running = false;
		} else {
			std::cout << "Error: unkown command: " << command << std::endl;
		}
	}
}

void renderCollisionBox(ed_Surface* s) {
	SDL_Rect collisionRect = { 
		s->x1 - deltaCamX, 
		s->y1 - deltaCamY, 
		s->x2 - s->x1, 
		s->y2 - s->y1 };

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

		SDL_SetRenderDrawColor(ed_mainRenderer, 255, 255, 255, 255);

		//render backgrounds
		for (ed_RenderObject& background : scene->backgrounds) {
			SDL_RenderCopy(ed_mainRenderer, background.getCurrentTexture(), NULL, background.getTextureBox());
		}

		//render foreground objects
		for (ed_RenderObject& texture : scene->foregroundObjects) {
			SDL_RenderCopy(ed_mainRenderer, texture.getCurrentTexture(), NULL, texture.getTextureBox());
		}

		if (!selectingSurface) {
			for (ed_Surface& s : scene->surfaces) {
				renderCollisionBox(&s);
			}
		} else {
			renderCollisionBox(editedSurface);
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