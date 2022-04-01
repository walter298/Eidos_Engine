#include "action_scene.h"

void ed_ActionScene::init()
{
	c_Player.setPos(this->pX, this->pY);

	ed_runningThreads.insert(ed_runningThreads.begin(), std::thread(ed_pMovementInput, 70));

	ed_runningThreads.insert(ed_runningThreads.begin(), std::thread(ed_checkPlayerCollision));
}

void ed_ActionScene::render()
{
	SDL_RenderCopy(ed_mainRenderer, c_Player.getCurrentTexture(), NULL, c_Player.getTextureBox());
}