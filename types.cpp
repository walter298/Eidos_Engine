#include "types.h"

bool ed_Button::hovered()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX > this->x1 && mouseX < this->x2 &&
		mouseY > this->y1 && mouseY < this->y2) 
	{
		return true;
	}

	return false;
}

void ed_Texture::updateToCurrentTexture(size_t sheetIndex, size_t textureIndex)
{
	//our current render position
	int CRX = this->renderGroups[this->sheetIndex][this->textureIndex].x;
	int CRY = this->renderGroups[this->sheetIndex][this->textureIndex].y;

	//set new texture to be at current position
	this->renderGroups[sheetIndex][textureIndex].x = CRX;
	this->renderGroups[sheetIndex][textureIndex].y = CRY;

	ed_Surface* currentSurface = &this->collisionGroups[this->sheetIndex][this->textureIndex];
	ed_Surface* updatedSurface = &this->collisionGroups[sheetIndex][textureIndex];

	//our current collision position
	size_t centerX = currentSurface->centerX;
	size_t centerY = currentSurface->centerY;

	int distX = updatedSurface->x2 - centerY;
	int distY = updatedSurface->y2 - centerY;

	//set collision constants to be at current position
	updatedSurface->centerX = centerX;
	updatedSurface->centerY = centerY;
	updatedSurface->x1 = centerX - distX;
	updatedSurface->y1 = centerY - distY;
	updatedSurface->x2 = centerX + distX;
	updatedSurface->y2 = centerY + distY;
}

void ed_Texture::updateToPosition(size_t sheetIndex, size_t textureIndex, int x, int y)
{
	auto* collisionBox = &this->collisionGroups[sheetIndex][textureIndex];
	auto* texture = &this->renderGroups[sheetIndex][textureIndex];

	int xDistance = collisionBox->centerX - collisionBox->x1;
	int yDistance = collisionBox->centerY - collisionBox->y1;

	collisionBox->centerX = x;
	collisionBox->centerY = y - yDistance;
	collisionBox->x1 = x - xDistance;
	collisionBox->y1 = y - yDistance * 2;
	collisionBox->x2 = x + xDistance;
	collisionBox->y2 = y;

	texture->x = collisionBox->x1;
	texture->y = collisionBox->y1;
}

void ed_Player::jump() {
	int jumpTime = SDL_GetTicks() + 100;

	int jumpInterval = SDL_GetTicks() + 5;

	this->jumping = true;

	while (true) {
		if (SDL_GetTicks() == jumpTime) {
			this->jumping = false;

			break;
		}

		if (SDL_GetTicks() == jumpInterval) {
			jumpInterval += 5;

			this->worldMove(ed_Dir::NONE, ed_Dir::UP);

			if (!this->yCamLocked) {
				this->camMove(ed_Dir::NONE, ed_Dir::UP);
			}
		}
	}
}

void ed_RenderObject::scaleTexture(ed_Texture& texture, int width, int height)
{
	auto scaledRelativePosition = [](int p1, int m1, int m2) {
		double relativePosition = 1.0 * p1 / m1;

		return relativePosition * m2;
	};

	for (size_t i = 0; i < texture.sheets.size(); i++) {
		for (size_t j = 0; j < texture.sheets[i].size(); j++) {
			texture.renderGroups[i][j].x = scaledRelativePosition(texture.renderGroups[i][j].x, 1920, width);
			texture.renderGroups[i][j].y = scaledRelativePosition(texture.renderGroups[i][j].y, 1080, height);

			texture.renderGroups[i][j].w = scaledRelativePosition(texture.renderGroups[i][j].w, 1920, width);
			texture.renderGroups[i][j].h = scaledRelativePosition(texture.renderGroups[i][j].h, 1080, height);

			texture.collisionGroups[i][j].y1 = scaledRelativePosition(texture.collisionGroups[i][j].y1, 1920, width);
			texture.collisionGroups[i][j].y2 = scaledRelativePosition(texture.collisionGroups[i][j].y2, 1080, height);

			texture.collisionGroups[i][j].x1 = scaledRelativePosition(texture.collisionGroups[i][j].x1, 1920, width);
			texture.collisionGroups[i][j].x2 = scaledRelativePosition(texture.collisionGroups[i][j].x2, 1080, height);

			texture.collisionGroups[i][j].centerX = scaledRelativePosition(texture.collisionGroups[i][j].centerX, 1920, width);
			texture.collisionGroups[i][j].centerY = scaledRelativePosition(texture.collisionGroups[i][j].centerY, 1080, height);
		}
	}

	texture.deltaCamX = scaledRelativePosition(texture.deltaCamX, 1920, width);
	texture.deltaCamY = scaledRelativePosition(texture.deltaCamY, 1080, height);

	texture.deltaWorldX = scaledRelativePosition(texture.deltaWorldX, 1920, width);
	texture.deltaWorldY = scaledRelativePosition(texture.deltaWorldY, 1080, height);
}