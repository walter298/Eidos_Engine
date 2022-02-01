#include "collision.h"

ed_Surface getNearestFloor(ed_Texture tex)
{
	std::vector<ed_Surface> surfacesBelow;

	for (ed_Surface surface : ed_globalScene.surfaces)
	{
		if (surface.x1 < ed_Player.tex.world.x && surface.x2 > ed_Player.tex.world.x) {
			surfacesBelow.push_back(surface);
		}
	}

	ed_Surface highestFloor = { 0, 7000, 0, 0 };

	for (ed_Surface surface : surfacesBelow) {
		if (surface.y1 < highestFloor.y1) {
			highestFloor = surface;
		}
	}

	return highestFloor;
}

void ed_updateSurfaceBelowPlayer()
{
	while (ed_globalScene.executing) {
		ed_Player.surfaceBelow = getNearestFloor(ed_Player.tex);

		//std::cout << ed_Player.tex.world.y << std::endl;
	}
}

ed_Surface getNearestWall(ed_Texture tex)
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
		while (ed_Player.tex.world.y < ed_Player.surfaceBelow.y1) {
			if (ed_Player.tex.world.y + ed_Player.tex.deltaWorldY > ed_Player.surfaceBelow.y1) {
				ed_Player.tex.world.y = ed_Player.surfaceBelow.y1;
				ed_Player.tex.ren.y = ed_Player.surfaceBelow.y1;

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