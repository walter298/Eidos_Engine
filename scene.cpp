#include "scene.h"

ed_Scene* ed_globalScene;

void ed_scaleScene(ed_Scene& scene)
{
	int newWindowWidth, newWindowHeight;

	newWindowWidth = 1920, newWindowHeight = 1080;

	auto highestMeasurement = [](int measurement, int newMeasurement, int increment) {
		while (true) {
			if (newMeasurement + increment > measurement) {
				return newMeasurement;
			}

			newMeasurement += increment;
		}
	};

	auto scaledRelativePosition = [](int p1, int m1, int m2) {
		double relativePosition = 1.0 * p1 / m1;

		return relativePosition * m2;
	};

	newWindowWidth = highestMeasurement(ed_screenWidth, newWindowWidth, 16);
	newWindowHeight = highestMeasurement(ed_screenHeight, newWindowHeight, 9);

	SDL_SetWindowSize(ed_mainWindow, newWindowWidth, newWindowHeight);
	SDL_SetWindowPosition(ed_mainWindow, 0, 0);

	//change background sizes
	for (ed_Texture& t : scene.backgrounds) {
		t.renderGroups[t.sheetIndex][t.textureIndex].w = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].w, 1920, newWindowWidth);
		t.renderGroups[t.sheetIndex][t.textureIndex].h = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].h, 1080, newWindowHeight);

		t.renderGroups[0][0].x = scaledRelativePosition(t.renderGroups[0][0].x, 1920, newWindowWidth);
	}

	for (ed_Texture& t : scene.foregroundObjects) {
		for (size_t i = 0; i < t.sheets.size(); i++) {
			for (size_t j = 0; j < t.sheets[i].size(); j++) {
				t.renderGroups[i][j].x = scaledRelativePosition(t.renderGroups[i][j].x, 1920, newWindowWidth);
				t.renderGroups[i][j].y = scaledRelativePosition(t.renderGroups[i][j].y, 1080, newWindowHeight);

				t.renderGroups[i][j].w = scaledRelativePosition(t.renderGroups[i][j].w, 1920, newWindowWidth);
				t.renderGroups[i][j].h = scaledRelativePosition(t.renderGroups[i][j].h, 1080, newWindowHeight);

				t.collisionGroups[i][j].y1 = scaledRelativePosition(t.collisionGroups[i][j].y1, 1920, newWindowWidth);
				t.collisionGroups[i][j].y2 = scaledRelativePosition(t.collisionGroups[i][j].y2, 1080, newWindowHeight);

				t.collisionGroups[i][j].x1 = scaledRelativePosition(t.collisionGroups[i][j].x1, 1920, newWindowWidth);
				t.collisionGroups[i][j].x2 = scaledRelativePosition(t.collisionGroups[i][j].x2, 1080, newWindowHeight);
			}
		}

		t.deltaCamX = scaledRelativePosition(t.deltaCamX, 1920, newWindowWidth);
		t.deltaCamY = scaledRelativePosition(t.deltaCamY, 1080, newWindowHeight);

		t.deltaWorldX = scaledRelativePosition(t.deltaWorldX, 1920, newWindowWidth);
		t.deltaWorldY = scaledRelativePosition(t.deltaWorldY, 1080, newWindowHeight);
	}

	for (ed_Surface& surface : scene.surfaces) {
		surface.x1 = scaledRelativePosition(surface.x1, 1920, newWindowWidth);
		surface.x2 = scaledRelativePosition(surface.x2, 1920, newWindowWidth);

		surface.y1 = scaledRelativePosition(surface.y1, 1080, newWindowHeight);
		surface.y2 = scaledRelativePosition(surface.y2, 1080, newWindowHeight);
	}

	for (ed_Button& button : scene.buttons) {
		button.x1 = scaledRelativePosition(button.x1, 1920, newWindowWidth);
		button.x2 = scaledRelativePosition(button.x2, 1920, newWindowWidth);

		button.y1 = scaledRelativePosition(button.y1, 1080, newWindowHeight);
		button.y2 = scaledRelativePosition(button.y2, 1080, newWindowHeight);
	}
}

void ed_parseButtons(ed_Scene& scene, std::vector<std::string> input)
{
	for (int stringIndex = 1; stringIndex < input.size(); stringIndex++) {
		if (input[stringIndex].empty()) {
			continue;
		}

		ed_Button newButton;

		size_t end, idIndex = input[stringIndex].size(); //the char right before the id

		std::string ID;

		while (true) {
			idIndex--;

			if (input[stringIndex][idIndex] == '_') {
				end = idIndex;

				break;
			}

			ID.insert(ID.begin(), input[stringIndex][idIndex]);
		}

		newButton.ID = std::stoi(ID);

		std::vector<int> newButtonCoords = ed_parseNums(ed_stringSection(input[stringIndex], 0, end - 1));

		newButton = { newButtonCoords[0], newButtonCoords[1], newButtonCoords[2], newButtonCoords[3] };

		scene.buttons.push_back(newButton);
	}
}

void ed_parseBackgroundImage(ed_Scene& scene, std::vector<std::string> input)
{
	for (size_t i = 1; i < input.size(); i++) {
		if (input[i].empty()) {
			continue;
		}

		std::string imagePath = input[i];

		ed_Texture background;

		background.sheets.push_back({});
		background.renderGroups.push_back({ {} });

		background.sheets[0].push_back({ IMG_LoadTexture(ed_mainRenderer, imagePath.c_str()) });

		int backgroundIndex = scene.backgrounds.size();

		background.renderGroups[0][0] = { 1920 * backgroundIndex, 0, 1920, 1080 };

		scene.backgrounds.push_back(background);
	}
}

void ed_enterSceneData(ed_Scene& scene, std::string fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	if (!file.is_open()) {
		std::cout << "Error, could not open file: " << fileName << std::endl;

		return;
	}

	std::vector<std::vector<std::string>> dataPieces; //each piece of data being parsed, such as surfaces objects, background pics, etc

	std::string line;

	std::vector<std::string> currentData;

	while (std::getline(file, line)) {
		if (line.empty()) { //ignore lines where nothing is written
			continue;
		}

		if (line == "end") {
			dataPieces.push_back(currentData);
			currentData.clear();

			continue;
		}

		currentData.push_back(line);
	}

	for (std::vector<std::string>& dataPiece : dataPieces) {
		if (dataPiece[0].empty()) {
			continue;
		}

		if (dataPiece[0] == "BACKGROUND_IMAGES") {
			ed_parseBackgroundImage(scene, dataPiece);
		}
		else if (dataPiece[0] == "BUTTONS") {
			ed_parseButtons(scene, dataPiece);
		}
		else if (dataPiece[0] == "TEXTURE_PLACEMENT") {
			ed_Texture texture = ed_loadedTextures[std::stoi(dataPiece[1])];

			std::vector<int> nums = ed_parseNums(dataPiece[2]);

			texture.renderGroups[texture.sheetIndex][texture.textureIndex].x = nums[0];
			texture.renderGroups[texture.sheetIndex][texture.textureIndex].y = nums[1];

			int xRemainder = texture.collisionGroups[texture.sheetIndex][texture.textureIndex].x2 -
				texture.collisionGroups[texture.sheetIndex][texture.textureIndex].x1;
			int yRemainder = texture.collisionGroups[texture.sheetIndex][texture.textureIndex].y2 -
				texture.collisionGroups[texture.sheetIndex][texture.textureIndex].y1;

			texture.collisionGroups[texture.sheetIndex][texture.textureIndex].x1 = nums[0];
			texture.collisionGroups[texture.sheetIndex][texture.textureIndex].x2 = nums[0] + xRemainder;

			texture.collisionGroups[texture.sheetIndex][texture.textureIndex].y1 = nums[1];
			texture.collisionGroups[texture.sheetIndex][texture.textureIndex].x1 = nums[1] + yRemainder;
		}
		else if (dataPiece[0] == "PLAYER_PLACEMENT") {
			std::vector<int> nums = ed_parseNums(dataPiece[1]);

			int x = nums[0], y = nums[1]; //our center point

			scene.pX = x;
			scene.pY = y;

			//c_Player.tex.updateToPosition(0, 0, x, y);
		}
		else if (dataPiece[0] == "FLOOR") {
			ed_Surface newFloor;

			std::vector<int> lengthPeriod = ed_parseNums(dataPiece[1]);

			std::vector<int> heightPeriod = ed_parseNums(dataPiece[2]);

			newFloor.x1 = lengthPeriod[0];
			newFloor.x2 = lengthPeriod[1];

			newFloor.y1 = heightPeriod[0];
			newFloor.y2 = heightPeriod[1];

			scene.surfaces.push_back(newFloor);
		}
		else {
			std::cout << "Error. Unknown object: " << dataPiece[0] << std::endl;

			return;
		}
	}
}

void ed_executeScene(ed_Scene* scene)
{
	scene->init();

	ed_globalScene = scene;

	ed_globalScene->init();
	
	const int fps = 60;

	ed_running = true;

	for (void(*method)() : ed_globalScene->backgroundMethods) {
		ed_runningThreads.insert(ed_runningThreads.begin(), (std::thread(method)));
	}

	for (std::thread& t : ed_runningThreads) {
		t.detach();
	}

	SDL_Event evt;

	const Uint8* mainKey = SDL_GetKeyboardState(NULL); //handles main inputs like quitting the game

	int waitTime;
	int endTime;

	while (ed_running) {
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				ed_running = false;
				break;
			}
		}

		SDL_RenderClear(ed_mainRenderer);

		waitTime = 1000 / fps;
		endTime = SDL_GetTicks() + waitTime;

		//render backgrounds
		for (ed_Texture& background : ed_globalScene->backgrounds) {
			SDL_RenderCopy(ed_mainRenderer, background.sheets[0][0], NULL, &background.renderGroups[0][0]);
		}

		//render foreground objects
		for (ed_Texture& texture : ed_globalScene->foregroundObjects) {
			SDL_RenderCopy(ed_mainRenderer, texture.sheets[texture.sheetIndex][texture.textureIndex], NULL,
				&texture.renderGroups[texture.sheetIndex][texture.textureIndex]);
		}

		ed_globalScene->render();

		while (SDL_GetTicks() + (1000 / fps) < endTime) {} //wait 

		SDL_RenderPresent(ed_mainRenderer);
	}
}




