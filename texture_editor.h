#ifndef TEXTURE_EDITOR
#define TEXTURE_EDITOR

#include <fstream>

#include "types.h"
#include "global.h"
#include "data_util.h"

class ed_RenderObjectCreator {
private:
	ed_RenderObject& obj;

	std::string path;

	void checkModeChange();
public:
	ed_RenderObjectCreator(ed_RenderObject& _obj, std::string _path)
		: obj(_obj), path(_path) {}

	void readData();

	void putCollisionData();
};

#endif