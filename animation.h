#pragma once

#include "global.h"

class ed_Animation {
protected:
	ed_RenderObject* tex;

	Uint32 spriteChangeInterval;
public:
	virtual void run() = 0;
};
