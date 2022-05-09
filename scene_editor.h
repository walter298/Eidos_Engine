#include <filesystem>

#include "scene.h"
#include "key_input.h"

class ed_SceneEditor {
private:
	ed_Scene* scene;
	std::string path;

	std::vector<ed_RenderObject*> allRenderObjects;

	void checkCamMovement();

	void drawRect(int& x1, int& y1, int& x2, int& y2);

	void injectSurface();

	void injectCamLockState();

	void getSelectedSurface();
	void editSurface();

	void getSelectedObject();
	void editObject();

	void getInput();

	void save();

	bool hasPlayer = false;
public:
	void editScene();

	ed_SceneEditor(ed_Scene* _scene, std::string _path, bool containsPlayer)
		: scene(_scene), path(_path) 
	{
		for (ed_RenderObject& s : scene->foregroundObjects) {
			allRenderObjects.push_back(&s);
		}

		if (containsPlayer) {
			allRenderObjects.push_back(&c_Player);
		}

		hasPlayer = containsPlayer;
	}
};