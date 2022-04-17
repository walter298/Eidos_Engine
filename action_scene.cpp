#include "action_scene.h"

void ed_ActionScene::setTexturePositions()
{
	c_Player.setPos(this->pX, this->pY);
}

void ed_ActionScene::render()
{
	SDL_RenderCopy(ed_mainRenderer, c_Player.getCurrentTexture(), NULL, c_Player.getTextureBox());
}