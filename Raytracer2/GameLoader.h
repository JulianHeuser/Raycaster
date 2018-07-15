#pragma once

#include "GameLevel.h"

class GameLoader{
	GameLoader(){}
	static GameLevel* currentLevel;
public:
	static bool Loading;

	static void Init();

	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev, int level);

	static void Render(Graphics* gfx);
	static void Update();
};