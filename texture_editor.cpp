#include "texture_editor.h"

//int editedTextureGroupIndex = 0;
//
//int editedTextureIndex = 0;
//int editedSpriteIndex = 0;

SDL_Rect collisionRect = {500, 500, 200, 200};

SDL_Rect dimensionRect = { 500, 500, 200, 200 };

bool checking = false;

std::vector<std::string> textureCollisionData;
std::vector<std::string> textureDimensionData;

bool applyAll = false;

std::fstream textureFile;

bool running = true;

void ed_RenderObjectCreator::readData()
{
	std::ifstream file;
	file.open(this->path.c_str());

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

			std::vector<SDL_Rect> dimensionBoundaries;
			std::vector<ed_Surface> collisionBoundaries;

			for (size_t textureIndex = 1; textureIndex < group.size(); textureIndex++) { 
				newTextureGroup.push_back(IMG_LoadTexture(ed_mainRenderer, group[textureIndex].c_str())); //load texture image

				//empty initializer values
				dimensionBoundaries.push_back({});
				collisionBoundaries.push_back({});
			}

			this->obj.sheets.push_back(newTextureGroup);
			this->obj.renderGroups.push_back(dimensionBoundaries);
			this->obj.collisionGroups.push_back(collisionBoundaries);
		} else if (group[0] == "ID") {
			this->obj.ID = std::stoi(group[1]);
		} else if (group[0] == "COLLISION") {
			for (size_t i = 1; i < group.size(); i++) {
				std::vector<int> nums = ed_parseNums(group[i]);

				this->obj.collisionGroups[nums[0]][nums[1]] = { nums[2], nums[3], nums[4], nums[5], nums[6], nums[7] };
			}
		} else if (group[0] == "DIMENSIONS") {
			for (size_t i = 1; i < group.size(); i++) {
				std::vector<int> nums = ed_parseNums(group[i]);

				this->obj.renderGroups[nums[0]][nums[1]] = { nums[2], nums[3], nums[4], nums[5] };
			}
		} else if (group[0] == "SPEED") {
			std::vector<int> nums = ed_parseNums(group[1]);

			this->obj.deltaWorldX = nums[0];
			this->obj.deltaWorldY = nums[1];
			this->obj.deltaCamX = nums[2];
			this->obj.deltaCamY = nums[3];
		}
	}
}

void checkDimensionChange()
{
	SDL_Event evt;

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	checking = true;

	while (checking) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (keyState[SDL_SCANCODE_RIGHT]) {
					dimensionRect.w += 5;
				} else if (keyState[SDL_SCANCODE_LEFT]) {
					dimensionRect.w -= 5;
				} else if (keyState[SDL_SCANCODE_UP]) {
					dimensionRect.h -= 5;
				} else if (keyState[SDL_SCANCODE_DOWN]) {
					dimensionRect.h += 5;
				} else if (keyState[SDL_SCANCODE_L]) {
					dimensionRect.x += 5;
				} else if (keyState[SDL_SCANCODE_K]) {
					dimensionRect.x -= 5;
				} else if (keyState[SDL_SCANCODE_I]) {
					dimensionRect.y -= 5;
				} else if (keyState[SDL_SCANCODE_M]) {
					dimensionRect.y += 5;
				}
			}
		}
	}
}

void checkCollisionChange()
{
	SDL_Event evt;

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	checking = true;

	while (checking) {
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (keyState[SDL_SCANCODE_RIGHT]) {
					collisionRect.w += 5;
				} else if (keyState[SDL_SCANCODE_LEFT]) {
					collisionRect.w -= 5;
				} else if (keyState[SDL_SCANCODE_UP]) {
					collisionRect.h -= 5;
				} else if (keyState[SDL_SCANCODE_DOWN]) {
					collisionRect.h += 5;
				} else if (keyState[SDL_SCANCODE_L]) {
					collisionRect.x += 5;
				} else if (keyState[SDL_SCANCODE_K]) {
					collisionRect.x -= 5;
				} else if (keyState[SDL_SCANCODE_I]) {
					collisionRect.y -= 5;
				} else if (keyState[SDL_SCANCODE_M]) {
					collisionRect.y += 5;
				}
			}
		}
	}
}

void ed_RenderObjectCreator::saveTexture(size_t textureIndex)
{
	std::string cSheetIndex = std::to_string(this->obj.getSheetIndex());
	std::string cTextureIndex = std::to_string(textureIndex);

	std::string x = std::to_string(dimensionRect.x);
	std::string y = std::to_string(dimensionRect.y);

	std::string width = std::to_string(dimensionRect.w);
	std::string height = std::to_string(dimensionRect.h);

	std::string dimensionDataPiece;

	dimensionDataPiece.append(cSheetIndex);
	dimensionDataPiece.append("_");
	dimensionDataPiece.append(cTextureIndex);
	dimensionDataPiece.append("_");
	dimensionDataPiece.append(x);
	dimensionDataPiece.append("_");
	dimensionDataPiece.append(y);
	dimensionDataPiece.append("_");
	dimensionDataPiece.append(width);
	dimensionDataPiece.append("_");
	dimensionDataPiece.append(height);

	textureDimensionData.push_back(dimensionDataPiece);

	std::string col_x = std::to_string(collisionRect.x);
	std::string col_y = std::to_string(collisionRect.y);
	std::string col_x2 = std::to_string(collisionRect.x + collisionRect.w);
	std::string col_y2 = std::to_string(collisionRect.y + collisionRect.h);
	std::string centerX = std::to_string(collisionRect.x + collisionRect.w / 2);
	std::string centerY = std::to_string(collisionRect.y + collisionRect.h / 2);

	std::string collisionDataPiece;

	collisionDataPiece.append(cSheetIndex);
	collisionDataPiece.append("_");
	collisionDataPiece.append(cTextureIndex);
	collisionDataPiece.append("_");
	collisionDataPiece.append(col_x);
	collisionDataPiece.append("_");
	collisionDataPiece.append(col_y);
	collisionDataPiece.append("_");
	collisionDataPiece.append(col_x2);
	collisionDataPiece.append("_");
	collisionDataPiece.append(col_y2);
	collisionDataPiece.append("_");
	collisionDataPiece.append(centerX);
	collisionDataPiece.append("_");
	collisionDataPiece.append(centerY);

	textureCollisionData.push_back(collisionDataPiece);
}

void ed_RenderObjectCreator::checkModeChange()
{
	std::string input;

	std::thread currentThread;

	while (true) {
		std::cin >> input;

		checking = false;
		
		if (input == "collision") {
			currentThread = std::thread(checkCollisionChange);
			currentThread.detach();
		} else if (input == "dimensions") {
			currentThread = std::thread(checkDimensionChange);
			currentThread.detach();
		} else if (input == "change_texture") {
			std::cout << "changing texture\n";

			int cSheetIndex, cTextureIndex;

			std::cout << "enter spritesheet number\n";
			std::cin >> cSheetIndex;

			std::cout << "enter texture index:";
			std::cin >> cTextureIndex;

			if (!(cSheetIndex < 0) && !(cSheetIndex >= this->obj.sheets.size())) {
				if (!(cTextureIndex < 0) && !(cTextureIndex >= this->obj.sheets[cSheetIndex].size())) {
					this->obj.changeTexture(cSheetIndex, cTextureIndex);
				} else {
					std::cout << "Error: texture index out of range\n";
				}
			} else {
				std::cout << "Error: spritesheet index out of range\n";
			} 
		}
		else if (input == "save_texture") {
			std::cout << "saving texture...\n";

			saveTexture(this->obj.getTextureIndex());

			std::cout << "texture successfully saved\n";
		} else if (input == "save_sprite_sheet") {
			std::cout << "saving sprite sheet...\n";

			for (unsigned int i = 0; i < this->obj.sheets[this->obj.getSheetIndex()].size(); i++) {
				saveTexture(i);
			}

			std::cout << "sprite sheet successfully saved\n";

		} else if (input == "save_exit") {
			std::cout << "saving texture...\n";

			textureFile << "";
			textureFile << "COLLISION\n";

			for (std::string s : textureCollisionData) {
				textureFile << s.c_str() << std::endl;
			}

			textureFile << "\nend\n";

			textureFile << "DIMENSIONS\n";

			for (std::string s : textureDimensionData) {
				textureFile << s.c_str() << std::endl;
			}

			textureFile << "\nend\n";

			textureFile.close();

			std::cout << "saved successfully\n";

			running = false;
		} else {
			std::cout << "Error: unknown command: " << input << std::endl;
		}
	}
}

void ed_RenderObjectCreator::writeData()
{
	textureFile.open(this->path.c_str(), std::ios::app);

	this->readData();

	if (!textureFile.is_open()) {
		std::cout << "Error: " << "could not open " << this->path.c_str() << std::endl;

		return;
	}

	std::thread modeCheck(&ed_RenderObjectCreator::checkModeChange, this);
	modeCheck.detach();

	SDL_Event evt;

	while (running) {
		while (SDL_PollEvent(&evt)) {}

		SDL_RenderClear(ed_mainRenderer);

		SDL_SetRenderDrawColor(ed_mainRenderer, 0xFF, 0, 0, 0xFF);
		
		SDL_RenderDrawPoint(ed_mainRenderer, 50, 50);

		SDL_RenderDrawRect(ed_mainRenderer, &collisionRect);

		SDL_SetRenderDrawColor(ed_mainRenderer, 255, 255, 255, 255);

		SDL_RenderCopy(ed_mainRenderer, this->obj.getCurrentTexture(), NULL, &dimensionRect);

		SDL_RenderPresent(ed_mainRenderer);

		SDL_Delay(10);
	}
}