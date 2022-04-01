#include "collision.h"

ed_Surface getNearestFloor(ed_RenderObject* obj)
{
	std::vector<ed_Surface> surfacesBelow;

	for (const ed_Surface surface : ed_globalScene->surfaces)
	{
		if (surface.x1 < obj->getCollisionBox().x1 && 
			surface.x2 > obj->getCollisionBox().x2 && 
			surface.y1 >= obj->getCollisionBox().y2) 
		{
			surfacesBelow.push_back(surface);
		} 
	}

	ed_Surface highestFloor;
	highestFloor = { 0, 7000, 5000, 0, 0, 0 };

	for (ed_Surface surface : surfacesBelow) {
		if (surface.y1 < highestFloor.y1) {
			highestFloor = surface;
		}
	}

	return highestFloor;
}

void ed_updateSurfaceBelowPlayer()
{
	while (ed_running) {
		c_Player.surfaceBelow = getNearestFloor(&c_Player);

		SDL_Delay(20);
	}
}

ed_Surface getNearestWall(ed_Texture tex)
{
	return {};
}

void updateNearestWall()
{
	while (ed_running) {
		
	}
}

void ed_checkPlayerCollision()
{
	std::thread updateSurfaceBelow(ed_updateSurfaceBelowPlayer);
	updateSurfaceBelow.detach();
	 
	ed_Surface currentSurface; //used so we are not constantly accessing updating texture

	while (ed_running) 
	{
		//if we are jumping than we don't check for gravity
		if (c_Player.jumping) {
			continue;
		}

		currentSurface = getNearestFloor(&c_Player);

		//if we already are ground we don't check for gravity
		if (c_Player.getCollisionBox().y2 == currentSurface.y1) {
			continue;
		}

		while (true) {
			if (c_Player.getCollisionBox().y2 >= currentSurface.y1) {
				std::cout << "hit ground\n";
				c_Player.setPos(c_Player.getCollisionBox().centerX, currentSurface.y1);

				break;
			}

			//fall 
			c_Player.worldMove(ed_Dir::NONE, ed_Dir::DOWN);

			if (!c_Player.yCamLocked) {
				c_Player.camMove(ed_Dir::NONE, ed_Dir::DOWN);
			}

			SDL_Delay(5);
		}
	}
}

void updateVerticalPlayerMovement()
{

}