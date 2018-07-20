#pragma once

#include "Renderer.h"

static bool Loading;
class Loader{
public:
	static int currentLevel;


	void LoadLevel(Renderer* ren, int level);
	void ResetLevel(Renderer* ren);
	void LoadInitialLevel(Renderer* ren, int initialLevel);
};