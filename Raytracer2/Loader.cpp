#include "Loader.h"
#include "Renderer.h"

int Loader::currentLevel;

void Loader::LoadLevel(Renderer* ren, int level){
	//if (!Loading){
		Loading = true;
		ren->Unload();
		ren->Load(level);
		currentLevel = level;
	//}
}

void Loader::ResetLevel(Renderer* ren){

}

void Loader::LoadInitialLevel(Renderer* ren, int initialLevel){
	//if (!Loading){
		Loading = true;
		ren->Unload();
		ren->Load(initialLevel);
		currentLevel = initialLevel;
	//}
}