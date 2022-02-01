#include "scene_data.h"

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
		std::cout << t.ren.x << ", " << t.ren.y << ", " << t.ren.w << ", " << t.ren.h << std::endl;

		t.ren.x = scaledRelativePosition(t.ren.x, 1920, newWindowWidth);
		t.ren.y = scaledRelativePosition(t.ren.y, 1080, newWindowHeight);

		t.ren.w = scaledRelativePosition(t.ren.w, 1920, newWindowWidth);
		t.ren.h = scaledRelativePosition(t.ren.h, 1080, newWindowHeight);

		t.world.x = scaledRelativePosition(t.world.x, 1920, newWindowWidth);
		t.world.y = scaledRelativePosition(t.world.y, 1080, newWindowHeight);

		t.world.w = scaledRelativePosition(t.world.w, 1920, newWindowWidth);
		t.world.h = scaledRelativePosition(t.world.h, 1080, newWindowHeight);

		t.deltaCamX = scaledRelativePosition(t.deltaCamX, 1920, newWindowWidth);
		t.deltaCamY = scaledRelativePosition(t.deltaCamY, 1080, newWindowHeight);

		t.deltaWorldX = scaledRelativePosition(t.deltaWorldX, 1920, newWindowWidth);
		t.deltaWorldY = scaledRelativePosition(t.deltaWorldY, 1080, newWindowHeight);
	}

	if (scene.containsPlayer) {
		ed_Player.tex.ren.x = scaledRelativePosition(ed_Player.tex.ren.x, 1920, newWindowWidth);
		ed_Player.tex.ren.y = scaledRelativePosition(ed_Player.tex.ren.y, 1080, newWindowHeight);

		ed_Player.tex.ren.w = scaledRelativePosition(ed_Player.tex.ren.w, 1920, newWindowWidth);
		ed_Player.tex.ren.h = scaledRelativePosition(ed_Player.tex.ren.h, 1080, newWindowHeight);

		ed_Player.tex.world.x = scaledRelativePosition(ed_Player.tex.world.x, 1920, newWindowWidth);
		ed_Player.tex.world.y = scaledRelativePosition(ed_Player.tex.world.y, 1080, newWindowHeight);

		ed_Player.tex.world.w = scaledRelativePosition(ed_Player.tex.world.w, 1920, newWindowWidth);
		ed_Player.tex.world.h = scaledRelativePosition(ed_Player.tex.world.h, 1080, newWindowHeight);

		ed_Player.tex.deltaCamX = scaledRelativePosition(ed_Player.tex.deltaCamX, 1920, newWindowWidth);
		ed_Player.tex.deltaCamY = scaledRelativePosition(ed_Player.tex.deltaCamY, 1080, newWindowHeight);

		ed_Player.tex.deltaWorldX = scaledRelativePosition(ed_Player.tex.deltaWorldX, 1920, newWindowWidth);
		ed_Player.tex.deltaWorldY = scaledRelativePosition(ed_Player.tex.deltaWorldY, 1080, newWindowHeight);
	}

	for (ed_Surface& surface : scene.surfaces) {
		std::cout << surface.x1 << ", " << surface.x2 << ", " << surface.y1 << std::endl;

		surface.x1 = scaledRelativePosition(surface.x1, 1920, newWindowWidth);
		surface.x2 = scaledRelativePosition(surface.x2, 1920, newWindowWidth);

		surface.y1 = scaledRelativePosition(surface.y1, 1080, newWindowHeight);
		surface.y2 = scaledRelativePosition(surface.y2, 1080, newWindowHeight);

		std::cout << surface.x1 << ", " << surface.x2 << ", " << surface.y1 << std::endl;

		std::cout << "Window width: " << newWindowWidth << std::endl;
		std::cout << "Window height " << newWindowHeight << std::endl;
	}

	for (ed_Button& button : scene.buttons) {
		button.x1 = scaledRelativePosition(button.x1, 1920, newWindowWidth);
		button.x2 = scaledRelativePosition(button.x2, 1920, newWindowWidth);

		button.y1 = scaledRelativePosition(button.y1, 1080, newWindowHeight);
		button.y2 = scaledRelativePosition(button.y2, 1080, newWindowHeight);
	}
}

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
			ed_Surface newFloor;

			std::vector<int> lengthPeriod = ed_parseNums(dataPiece[1]);

			std::vector<int> heightPeriod = ed_parseNums(dataPiece[2]);

			newFloor.x1 = lengthPeriod[0];
			newFloor.x2 = lengthPeriod[1];

			newFloor.y1 = heightPeriod[0];

			scene.surfaces.push_back(newFloor);
		}
		else {
			std::cout << "Error. Unknown object: " << dataPiece[0] << std::endl;

			return;
		}
	}
}

