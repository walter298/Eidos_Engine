#include "collision.h"

ed_Surface getNearestFloor(ed_RenderObject* obj)
{
	std::vector<ed_Surface> surfacesBelow;

	for (const ed_Surface& surface : ed_globalScene->surfaces)
	{
		if ((surface.x1 <= obj->getCollisionBox().x1 && 
			surface.x2 >= obj->getCollisionBox().x2) && 
			surface.y1 >= obj->getCollisionBox().y2) 
		{
			surfacesBelow.push_back(surface);
		} 
	}

	ed_Surface highestFloor;
	highestFloor = { 0, 7000, 5000, 0, 0, 0 };

	for (ed_Surface& surface : surfacesBelow) {
		if (surface.y1 < highestFloor.y1) {
			highestFloor = surface;
		} 
	}

	return highestFloor;
}

void updateGround(ed_AnimateObject& obj, bool& updating)
{
	while (ed_running) {
		if (NEAREST_FLOOR != obj.surfaceBelow) {
			updating = true;
		}
	}
}

ed_Surface getNearestWall(ed_RenderObject* tex)
{
	return {};
}

void updateNearestWall()
{
	while (ed_running) {
		
	}
}

void ed_checkObjectCollision(ed_AnimateObject& obj)
{
	bool updating = false;
	bool accessing = false;

	std::thread updateSurfaceBelow(updateGround, std::ref(obj), std::ref(updating));
	updateSurfaceBelow.detach();

	obj.surfaceBelow = NEAREST_FLOOR;

	while (ed_running) 
	{
		//update floor
		if (updating) {
			obj.surfaceBelow = NEAREST_FLOOR;

			updating = false;
		}

		//if we are jumping, floating, etc
		if (obj.traveling) {
			continue;
		}

		//if we are already on the ground 
		if (obj.getY2() == obj.surfaceBelow.y1) {
			continue;
		} 

		//enter freefall until we hit ground
		obj.falling = true;

		while (true) {
			ed_Surface s = obj.surfaceBelow;

			//fall
			obj.worldMove(ed_Dir::NONE, ed_Dir::DOWN);

			if (!obj.yCamLocked) {
				obj.camMove(ed_Dir::NONE, ed_Dir::DOWN);
			} else {
				ed_globalScene->moveCam(0, -obj.getDeltaCamY());
			}

			//if we have hit or passed the ground set object to be surface level
			if (obj.getY2() >= s.y1) { 
				obj.setPos(obj.getCX(), s.y1 - (obj.getY2() - obj.getCY()));

				break;
			}

			//if surface below object has changed, update it
			if (updating) {
				obj.surfaceBelow = NEAREST_FLOOR;

				updating = false;
			}

			SDL_Delay(10);
		}

		obj.falling = false;
 	}
}

void updateVerticalPlayerMovement()
{

}
