#include <fstream>
#include <string>
#include <iostream>
#include <thread>

#include "global.h"
#include "data_util.h"

void ed_enterTextureData(ed_Texture& texture, std::string fileName);
void ed_enterSceneData(ed_Scene& scene, std::string fileName);

void ed_scaleScene(ed_Scene& scene);