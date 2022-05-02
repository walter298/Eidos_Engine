#ifndef ED_SCENE
#define ED_SCENE

#include <fstream>
#include <string>
#include <iostream>
#include <thread>

#include "global.h"
#include "data_util.h"

class ed_SceneEditor;

class ed_Scene 
{
protected:
	virtual void render() = 0;

	virtual void setTexturePositions() = 0;
	virtual void initThreads() = 0;
public:
	std::vector<ed_RenderObject> backgrounds;

	std::vector<ed_RenderObject> foregroundObjects;

	std::vector<void(*)()> backgroundMethods;

	std::vector<ed_Surface> surfaces;

	std::vector<ed_Button> buttons;

	int pX = 0, pY = 0;

	void execute();

	void readData(std::string fileName);

	void parseBackgroundImage(std::vector<std::string> input);

	void parseButtons(std::vector<std::string> input);

	void ed_scaleScene(ed_Scene& scene);

	void(*customInit)() = NULL;

	friend class ed_SceneEditor;
};

extern ed_Scene* ed_globalScene;

#endif