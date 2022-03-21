#include "collision.h"

ed_Surface getNearestFloor(ed_Texture tex)
{
	std::vector<ed_Surface> surfacesBelow;

	auto c = &c_Player.tex;

	for (ed_Surface surface : ed_globalScene.surfaces)
	{
		if (surface.x1 < c->collisionGroups[c->sheetIndex][c->textureIndex].x1 && 
			surface.x2 > c->collisionGroups[c->sheetIndex][c->textureIndex].x2 && 
			surface.y1 >= c->collisionGroups[c->sheetIndex][c->textureIndex].y2) 
		{
			surfacesBelow.push_back(surface);
		}
	}

	ed_Surface highestFloor;
	highestFloor.init(0, 7000, 0, 0, 0, 0);

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
		c_Player.surfaceBelow = getNearestFloor(c_Player.tex);
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
		if (c_Player.jumping) {
			continue;
		}

		if (c_Player.tex.collisionGroups[c_Player.tex.sheetIndex][c_Player.tex.textureIndex].y2 < c_Player.surfaceBelow.y1) {
			c_Player.falling = true;

			while (true) {
				auto c = &c_Player.tex;

				if (c->collisionGroups[c->sheetIndex][c->textureIndex].y2 + c->deltaWorldY >= c_Player.surfaceBelow.y1 
					&& !c_Player.jumping) 
				{
					/*automatically set texture's y position to be on top of floor if we overshoot*/
					c->collisionGroups[c->sheetIndex][c->textureIndex].y1 = 
						c_Player.surfaceBelow.y1 - (c->collisionGroups[c->sheetIndex][c->textureIndex].y2 - c->collisionGroups[c->sheetIndex][c->textureIndex].y1);
					c->collisionGroups[c->sheetIndex][c->textureIndex].y2 = c_Player.surfaceBelow.y1;
					
					c->renderGroups[c->sheetIndex][c->textureIndex].y = c_Player.surfaceBelow.y1;

					c_Player.falling = false;

					break;
				}

				c->collisionGroups[c->sheetIndex][c->textureIndex].y1 += c->deltaWorldY;
				c->collisionGroups[c->sheetIndex][c->textureIndex].y2 += c->deltaWorldY;

				c->renderGroups[c->sheetIndex][c->textureIndex].y += c->deltaCamY;

				SDL_Delay(5);
			}
		}
	}
}

void updatePlayerGravity()
{
	
}

void updateVerticalPlayerMovement()
{

}