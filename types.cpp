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

void ed_ScaleTexture(ed_Texture& texture, int width, int height)
{
	texture.ren.w += width;
	texture.ren.h += height;
}

int ed_Character::getSheetIndex()
{
	return this->tex.sheetIndex;
}

int ed_Character::getTextureIndex()
{
	return this->tex.textureIndex;
}