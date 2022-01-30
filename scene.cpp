#include "scene.h"

std::string ed_stringSection(std::string s, size_t start, size_t end)
{
	std::string section;

	for (size_t i = start; i <= end; i++) {
		section.push_back(s[i]);
	}

	return section;
}

std::vector<int> ed_parseNums(std::string input)
{
	std::vector<int> nums;

	int lastUnderscoreIndex = -1;

	for (int charIndex = 0; charIndex <= input.size(); charIndex++) {
		if (input[charIndex] == '_' || charIndex == input.size()) {
			std::string currentButtonString;

			for (int numIndex = lastUnderscoreIndex + 1; numIndex < charIndex; numIndex++) {
				currentButtonString.push_back(input[numIndex]);
			}

			nums.push_back(std::stoi(currentButtonString));

			lastUnderscoreIndex = charIndex;
		}
	}

	return nums;
}

void ed_enterTextureData(ed_Texture& texture, std::string fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::vector<std::vector<std::string>> dataPieces;
	std::vector<std::string> currentData;

	std::string currentLine;

	while (std::getline(file, currentLine)) {
		if (currentLine.empty()) {
			continue;
		}

		if (currentLine == "end") {
			dataPieces.push_back(currentData);
			currentData.clear();

			continue;
		}

		currentData.push_back(currentLine);
	}

	for (std::vector<std::string>& group : dataPieces) {
		if (group[0] == "SPRITESHEET") {
			std::vector<SDL_Texture*> newTextureGroup;

			for (size_t textureIndex = 1; textureIndex < group.size(); textureIndex++) {
				newTextureGroup.push_back(IMG_LoadTexture(ed_mainRenderer, group[textureIndex].c_str()));
			}

			texture.sheets.push_back(newTextureGroup);

		}
		else if (group[0] == "RECT") {
			std::vector<int> rectStats = ed_parseNums(group[1]);

			texture.ren = { rectStats[0], rectStats[1], rectStats[2], rectStats[3] };

		}
		else if (group[0] == "ID") {
			texture.ID = std::stoi(group[1]);
		}
		else {
			std::cout << "Error: type not found: " << group[0] << std::endl;
		}
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
		std::cout << input[i] << std::endl;

		std::string imagePath = input[i];

		ed_Texture background;

		background.sheets.push_back({});

		background.sheets[0].push_back({ IMG_LoadTexture(ed_mainRenderer, imagePath.c_str()) });

		int backgroundIndex = scene.backgrounds.size();

		background.ren = { 1920 * backgroundIndex, 0, 1920, 1080 };

		scene.backgrounds.push_back(background);
	}
}

void ed_parseSurface(ed_Scene& scene, std::vector<std::string> input)
{

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

			texture.ren.x = nums[0];
			texture.ren.y = nums[1];
			texture.world.x = nums[0];
			texture.world.y = nums[1];
		}
		else if (dataPiece[0] == "PLAYER_PLACEMENT") {
			std::vector<int> nums = ed_parseNums(dataPiece[1]);

			ed_Player.tex.ren.x = nums[0];
			ed_Player.tex.ren.y = nums[0];

			ed_Player.tex.world.x = nums[0];
			ed_Player.tex.world.y = nums[0];
		}
		else if (dataPiece[0] == "FLOOR") {
			std::cout << "parsing floor\n";
			SDL_Rect newFloor;

			std::vector<int> lengthPeriod = ed_parseNums(dataPiece[1]);

			std::vector<int> heightPeriod = ed_parseNums(dataPiece[2]);

			newFloor.x = lengthPeriod[0];
			newFloor.w = lengthPeriod[1];

			newFloor.y = heightPeriod[0];

			scene.surfaces.push_back(newFloor);
		}
		else {
			std::cout << "Error. Unknown object: " << dataPiece[0] << std::endl;

			return;
		}
	}
}

void ed_checkButtonInputs()
{
	SDL_Event evt;

	if (ed_globalScene.buttons.size() == 0) {
		return;
	}

	std::cout << ed_globalScene.buttons.size() << std::endl;

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
		t.ren.w = newWindowWidth;
		t.ren.h = newWindowHeight;

		t.ren.x = scaledRelativePosition(t.ren.x, 1920, newWindowWidth);
	}

	for (ed_Texture& t : scene.foregroundObjects) {
		std::cout << "scaling textures\n";
		std::cout << t.ren.x << ", " << t.ren.y << ", " << t.ren.w << ", " << t.ren.h << std::endl;

		t.ren.x = scaledRelativePosition(t.ren.x, 1920, newWindowWidth);
		t.ren.y = scaledRelativePosition(t.ren.y, 1080, newWindowHeight);

		t.ren.w = scaledRelativePosition(t.ren.w, 1920, newWindowWidth);
		t.ren.h = scaledRelativePosition(t.ren.h, 1080, newWindowHeight);

		t.world.x = scaledRelativePosition(t.world.x, 1920, newWindowWidth);
		t.world.y = scaledRelativePosition(t.world.y, 1080, newWindowHeight);

		t.world.w = scaledRelativePosition(t.world.w, 1920, newWindowWidth);
		t.world.h = scaledRelativePosition(t.world.h, 1080, newWindowHeight);

		std::cout << t.ren.x << ", " << t.ren.y << ", " << t.ren.w << ", " << t.ren.h << std::endl;
	}

	for (ed_Button& button : scene.buttons) {
		button.x1 = scaledRelativePosition(button.x1, 1920, newWindowWidth);
		button.x2 = scaledRelativePosition(button.x2, 1080, newWindowWidth);

		button.y1 = scaledRelativePosition(button.y1, 1920, newWindowHeight);
		button.y2 = scaledRelativePosition(button.y2, 1080, newWindowHeight);
	}
}

void ed_executeScene(ed_Scene scene)
{
	ed_globalScene = scene;

	std::vector<std::thread> threads;

	for (void(*method)() : ed_globalScene.backgroundMethods) {
		threads.insert(threads.begin(), (std::thread(method)));
		threads[0].detach();
	}

	ed_globalScene.executing = true;

	//std::thread playerVerticalCollisionCheck(ed_checkPlayerCollision);

	if (ed_globalScene.containsPlayer) {
		threads.insert(threads.begin(), std::thread(ed_pMovementInput, 70));
		threads[0].detach();

		threads.insert(threads.begin(), std::thread(ed_checkPlayerCollision));
		threads[0].detach();
	}

	ed_Player.deltaY = 5;

	std::thread buttonCheck(ed_checkButtonInputs);

	buttonCheck.detach();

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
			SDL_RenderCopy(ed_mainRenderer, background.sheets[0][0], NULL, &background.ren);
		}

		//render foreground objects
		for (ed_Texture& texture : ed_globalScene.foregroundObjects) {
			SDL_RenderCopy(ed_mainRenderer, texture.sheets[texture.sheetIndex][texture.textureIndex], NULL, &texture.ren);
		}

		if (ed_globalScene.containsPlayer) {
			SDL_RenderCopy(ed_mainRenderer, ed_Player.tex.sheets[ed_Player.tex.sheetIndex][ed_Player.tex.textureIndex], NULL, &ed_Player.tex.ren);
		}

		SDL_RenderPresent(ed_mainRenderer);

		SDL_Delay(10);
	}
}