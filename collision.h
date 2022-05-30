#ifndef COLLISION
#define COLLISION

#include <thread>
#include <mutex>

#include "scene.h"

#define NEAREST_FLOOR getNearestFloor(&obj)
#define OBJ_FLOOR obj.surfaceBelow

void ed_checkObjectCollision(ed_AnimateObject& obj);

#endif