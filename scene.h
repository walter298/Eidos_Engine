#ifndef ED_SCENE
#define ED_SCENE

#include <fstream>
#include <string>
#include <iostream>
#include <thread>

#include "global.h"
#include "data_util.h"

class ed_Scene 
{
protected:
	virtual void render() {};

	virtual void init() {};
public:
	std::vector<ed_Texture> backgrounds;

	std::vector<ed_Texture> foregroundObjects;

	std::vector<void(*)()> backgroundMethods;

	std::vector<ed_RenderObject> NPCS;

	std::vector<ed_Surface> surfaces;

	std::vector<ed_Button> buttons;

	int pX = 0, pY = 0;

	friend void ed_executeScene(ed_Scene& scene);
};

class ed_Menu : public ed_Scene
{
protected:
	void render() override {};

	void init() override;
};

extern ed_Scene ed_globalScene;

void ed_executeScene(ed_Scene& scene);

void ed_enterTextureData(ed_Texture& texture, std::string fileName);
void ed_enterSceneData(ed_Scene& scene, std::string fileName);

void ed_scaleScene(ed_Scene& scene);

#endif