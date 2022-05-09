#include "action_scene.h"

void ed_ActionScene::updateSceneCamLockState()
{
	//c_Player.xCamLocked = true;
	//c_Player.yCamLocked = true;
	while (ed_running) {
		
		for (ed_CamLockState& s : camLocks) {
			if (s.getX1() < c_Player.getCollisionBox().x1 && s.getX2() > c_Player.getCollisionBox().x2
				&& s.getY1() < c_Player.getCollisionBox().y1 && s.getY2() > c_Player.getCollisionBox().y2) 
			{
				//std::cout << "in cam lock state\n";

				c_Player.xCamLocked = s.getXLocked();
				c_Player.yCamLocked = s.getYLocked();
			} 
		}
	}

	/*std::cout << "camera coords\n";
				std::cout << s.getX1() << ", " << s.getY1() << std::endl;
				std::cout << s.getX2() << ", " << s.getY2() << std::endl;

				std::cout << "Player Coords: \n";
				std::cout << c_Player.getCollisionBox().x1 << ", " << c_Player.getCollisionBox().y1 << std::endl;
				std::cout << c_Player.getCollisionBox().x2 << ", " << c_Player.getCollisionBox().y2 << std::endl;*/
}

void ed_ActionScene::setTexturePositions()
{
	c_Player.setPos(this->pX, this->pY);
}

void ed_ActionScene::render()
{
	SDL_RenderCopy(ed_mainRenderer, c_Player.getCurrentTexture(), NULL, c_Player.getTextureBox());
}

void ed_ActionScene::initThreads()
{
	ed_runningThreads.push_back(std::thread(&ed_ActionScene::updateSceneCamLockState, this));
}