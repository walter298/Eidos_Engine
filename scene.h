#ifndef ED_SCENE
#define ED_SCENE

#include <fstream>
#include <string>
#include <iostream>
#include <thread>

#include "global.h"
#include "data_util.h"

class ed_SceneEditor;

class ed_CamLockState {
protected:
	bool xLocked = false;
	bool yLocked = false;

	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
public:
	ed_CamLockState(int& _x1, int& _y1, int& _x2, int& _y2, bool x, bool y)
		: x1(_x1), y1(_y1), x2(_x2), y2(_y2), xLocked(x), yLocked(y) {}

	ed_CamLockState() = default;

	bool getXLocked()
	{
		return xLocked;
	}

	bool getYLocked()
	{
		return yLocked;
	}

	int getX1()
	{
		return x1;
	}
	int getY1()
	{
		return y1;
	}
	int getX2()
	{
		return x2;
	}
	int getY2()
	{
		return y2;
	}
};

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

	std::vector<ed_CamLockState> camLocks;

	ed_CamLockState currentCamLockState;

	int pX = 0, pY = 0;

	void execute();

	void readData(std::string fileName);

	void parseBackgroundImage(std::vector<std::string> input);

	void parseButtons(std::vector<std::string> input);

	void moveCam(int deltaX, int deltaY);

	void ed_scaleScene(ed_Scene& scene);

	void(*customInit)() = NULL;

	friend class ed_SceneEditor;
};

extern ed_Scene* ed_globalScene;

#endif