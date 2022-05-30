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

void ed_printSurface(ed_Surface& s)
{
	std::cout << "x1: " << s.x1 << ", " << "y1: " << s.y1 << std::endl;
	std::cout << "x2: " << s.x2 << ", " << "y2: " << s.y2 << std::endl;
}