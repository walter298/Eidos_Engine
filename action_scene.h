#pragma once

#include "scene.h"
#include "collision.h"

class ed_ActionScene : public ed_Scene
{
protected:
	void render() override;

	void init() override;
};