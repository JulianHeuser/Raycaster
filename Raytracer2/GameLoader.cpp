#include "GameLoader.h"
#include "Graphics.h"

GameLevel* GameLoader::currentLevel;
bool GameLoader::Loading;

void GameLoader::Init(){
	Loading = true;
	currentLevel = 0;
}

void GameLoader::LoadInitialLevel(GameLevel* lev){
	Loading = true;
	currentLevel = lev;
	currentLevel->Load(1);
	Loading = false;
}

void GameLoader::SwitchLevel(GameLevel* lev,int level){
	Loading = true;
	currentLevel->Unload();
	lev->Load(level);
	delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameLoader::Render(Graphics* gfx){
	if (Loading) return;
	currentLevel->Render(gfx);
}

void GameLoader::Update(){
	if (Loading) return;
	currentLevel->Update();
}