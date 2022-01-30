#include "collision.h"

SDL_Rect getNearestFloor(ed_Texture tex)
{
	std::vector<SDL_Rect> surfacesBelow;

	for (SDL_Rect rect : ed_globalScene.surfaces) 
	{
		if (rect.x < ed_Player.tex.world.x && rect.x + rect.w > ed_Player.tex.world.x) {
			surfacesBelow.push_back(rect);
		}
	}

	SDL_Rect highestFloor = { 0, 7000, 0, 0 };

	for (SDL_Rect rect : surfacesBelow) {
		if (rect.y > highestFloor.h) {
			highestFloor = rect;
		}
	}

	//std::cout << highestFloor.x << ", " << highestFloor.x + highestFloor.w << std::endl;

	return highestFloor;
}

void ed_updateSurfaceBelowPlayer()
{
	while (ed_globalScene.executing) {
		ed_Player.surfaceBelow = getNearestFloor(ed_Player.tex);
	}
}

SDL_Rect getNearestWall(ed_Texture tex)
{
	return {};
}

void updateNearestWall()
{
	while (ed_globalScene.executing) {
		
	}
}

void ed_checkPlayerCollision()
{
	std::thread updateSurfaceBelow(ed_updateSurfaceBelowPlayer);
	updateSurfaceBelow.detach();

	while (ed_globalScene.executing) 
	{
		while (ed_Player.tex.world.y < ed_Player.surfaceBelow.y) {
			if (ed_Player.tex.world.y + ed_Player.tex.deltaWorldY > ed_Player.surfaceBelow.y) {
				ed_Player.tex.world.y = ed_Player.surfaceBelow.y;
				ed_Player.tex.ren.y = ed_Player.surfaceBelow.y;

				break;
			}

			ed_Player.tex.ren.y += ed_Player.tex.deltaCamY;
			ed_Player.tex.world.y += ed_Player.tex.deltaWorldY;

			SDL_Delay(5);
		}
	}
}

void updatePlayerGravity()
{
	
}

void updateVerticalPlayerMovement()
{

}