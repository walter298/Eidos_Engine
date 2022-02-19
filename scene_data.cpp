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
		t.renderGroups[t.sheetIndex][t.textureIndex].w = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].w, 1920, newWindowWidth);
		t.renderGroups[t.sheetIndex][t.textureIndex].h = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].h, 1080, newWindowHeight);

		t.renderGroups[0][0].x = scaledRelativePosition(t.renderGroups[0][0].x, 1920, newWindowWidth);
	}

	for (ed_Texture& t : scene.foregroundObjects) {
		t.renderGroups[t.sheetIndex][t.textureIndex].x = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].x, 1920, newWindowWidth);
		t.renderGroups[t.sheetIndex][t.textureIndex].y = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].y, 1080, newWindowHeight);

		t.renderGroups[t.sheetIndex][t.textureIndex].w = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].w, 1920, newWindowWidth);
		t.renderGroups[t.sheetIndex][t.textureIndex].h = scaledRelativePosition(t.renderGroups[t.sheetIndex][t.textureIndex].h, 1080, newWindowHeight);

		t.collisionGroups[t.sheetIndex][t.textureIndex].y1 = scaledRelativePosition(t.collisionGroups[t.sheetIndex][t.textureIndex].y1, 1920, newWindowWidth);
		t.collisionGroups[t.sheetIndex][t.textureIndex].y2 = scaledRelativePosition(t.collisionGroups[t.sheetIndex][t.textureIndex].y2, 1080, newWindowHeight);

		t.collisionGroups[t.sheetIndex][t.textureIndex].x1 = scaledRelativePosition(t.collisionGroups[t.sheetIndex][t.textureIndex].x1, 1920, newWindowWidth);
		t.collisionGroups[t.sheetIndex][t.textureIndex].x2 = scaledRelativePosition(t.collisionGroups[t.sheetIndex][t.textureIndex].x2, 1080, newWindowHeight);

		t.deltaCamX = scaledRelativePosition(t.deltaCamX, 1920, newWindowWidth);
		t.deltaCamY = scaledRelativePosition(t.deltaCamY, 1080, newWindowHeight);

		t.deltaWorldX = scaledRelativePosition(t.deltaWorldX, 1920, newWindowWidth);
		t.deltaWorldY = scaledRelativePosition(t.deltaWorldY, 1080, newWindowHeight);
	}

	if (scene.containsPlayer) {
		auto c = &c_Player.tex;

		c->renderGroups[c->sheetIndex][c->textureIndex].x = scaledRelativePosition(c->renderGroups[c->sheetIndex][c->textureIndex].x, 1920, newWindowWidth);
		c->renderGroups[c->sheetIndex][c->textureIndex].y = scaledRelativePosition(c->renderGroups[c->sheetIndex][c->textureIndex].y, 1080, newWindowHeight);

		c->renderGroups[c->sheetIndex][c->textureIndex].w = scaledRelativePosition(c->renderGroups[c->sheetIndex][c->textureIndex].w, 1920, newWindowWidth);
		c->renderGroups[c->sheetIndex][c->textureIndex].h = scaledRelativePosition(c->renderGroups[c->sheetIndex][c->textureIndex].h, 1080, newWindowHeight);

		c->collisionGroups[c->sheetIndex][c->textureIndex].y1 = scaledRelativePosition(c->collisionGroups[c->sheetIndex][c->textureIndex].y1, 1920, newWindowWidth);
		c->collisionGroups[c->sheetIndex][c->textureIndex].y2 = scaledRelativePosition(c->collisionGroups[c->sheetIndex][c->textureIndex].y2, 1080, newWindowHeight);

		c->collisionGroups[c->sheetIndex][c->textureIndex].x1 = scaledRelativePosition(c->collisionGroups[c->sheetIndex][c->textureIndex].x1, 1920, newWindowWidth);
		c->collisionGroups[c->sheetIndex][c->textureIndex].x2 = scaledRelativePosition(c->collisionGroups[c->sheetIndex][c->textureIndex].x2, 1080, newWindowHeight);

		c->deltaCamX = scaledRelativePosition(c->deltaCamX, 1920, newWindowWidth);
		c->deltaCamY = scaledRelativePosition(c->deltaCamY, 1080, newWindowHeight);

		c->deltaWorldX = scaledRelativePosition(c->deltaWorldX, 1920, newWindowWidth);
		c->deltaWorldY = scaledRelativePosition(c->deltaWorldY, 1080, newWindowHeight);
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
		} else if (dataPiece[0] == "BUTTONS") {
			ed_parseButtons(scene, dataPiece);
		} else if (dataPiece[0] == "TEXTURE_PLACEMENT") {
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
		} else if (dataPiece[0] == "PLAYER_PLACEMENT") {
			std::vector<int> nums = ed_parseNums(dataPiece[1]);

			c_Player.tex.renderGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x = nums[0];
			c_Player.tex.renderGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].y = nums[1];

			int xRemainder = c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x2 -
				c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x1;
			int yRemainder = c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].y2 -
				c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].y1;

			c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x1 = nums[0];
			c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x2 = nums[0] + xRemainder;

			c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].y1 = nums[1];
			c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].x1 = nums[1] + yRemainder;
		} else if (dataPiece[0] == "FLOOR") {
			ed_Surface newFloor;

			std::vector<int> lengthPeriod = ed_parseNums(dataPiece[1]);

			std::vector<int> heightPeriod = ed_parseNums(dataPiece[2]);

			newFloor.x1 = lengthPeriod[0];
			newFloor.x2 = lengthPeriod[1];

			newFloor.y1 = heightPeriod[0];

			scene.surfaces.push_back(newFloor);
		} else {
			std::cout << "Error. Unknown object: " << dataPiece[0] << std::endl;

			return;
		}
	}
}

