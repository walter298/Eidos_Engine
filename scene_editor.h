#include "scene.h"

class ed_SceneEditor {
private:
	ed_Scene* scene;
	std::string path;

	std::vector<ed_RenderObject*> allRenderObjects;
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
	}
};