#pragma once

#include "scene.h"

class ed_Menu : public ed_Scene
{
protected:
	void render() override {};

	void initThreads() override;	

	void setTexturePositions() override {};
};


