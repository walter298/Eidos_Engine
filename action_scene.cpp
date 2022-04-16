#include "action_scene.h"

void ed_ActionScene::init()
{
	c_Player.setPos(this->pX, this->pY);

	this->customInit();
}

void ed_ActionScene::render()
{
	SDL_RenderCopy(ed_mainRenderer, c_Player.getCurrentTexture(), NULL, c_Player.getTextureBox());
}