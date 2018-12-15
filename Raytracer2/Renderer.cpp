#include "Renderer.h"
#include "Graphics.h"
#include <math.h>
#include <string>

int Renderer::currentLevel;
bool Renderer::Loading;

//Levels
int level1[6][6] = { { 1, 1, 1, 1, 1, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 1, 1, 1, 1, 1 } };


int level2[6][6] = { { 1, 1, 1, 1, 1, 1 },
{ 1, 0, 0, 0, 1, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 1, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 1 },
{ 1, 1, 1, 1, 1, 1 } };

void Renderer::Reset(){
	playerX = 3.0;
	playerY = 3.0;
	playerRadiusX = 0.25;
	playerRadiusY = 0.25;

	planeX = 0;
	planeY = .66f;

	fov = 1;
	moveSpeed = 0.1;
	turnSpeed = .1;

	dirX = -1;
	dirY = 0;

	xChange = 0;
	yChange = 0;

	rays = 0; //How many rays have been drawn

	width = 500;
	height = 500;

}

void Renderer::Load(int level){
	Reset();
	currentLevel = level;
	switch (level)
	{
	case(1) :
		for (int i = 0; i < 6; i++){
			for (int o = 0; o < 6; o++){
				grid[i][o] = level1[i][o];
			}
		}
		Loading = false;
		break;
	case(2) :
		for (int i = 0; i < 6; i++){
			for (int o = 0; o < 6; o++){
				grid[i][o] = level2[i][o];
			}
		}
		Loading = false;
		break;
	default:
		Loading = false;
		break;
	}
}

void  Renderer::Unload(){
	
}

void Renderer::Update(){

	float oldDirX = dirX;
	float oldDirY = dirY;
	float oldPlaneX = planeX;
	int realPlayerX = floor(playerX);
	int realPlayerY = floor(playerY);
	if (GetAsyncKeyState(0x57)){
		yChange = moveSpeed * dirY;
		xChange = moveSpeed * dirX;
	}
	//case 0x53:	//S
	if (GetAsyncKeyState(0x53)){
		yChange = -moveSpeed * dirY;
		xChange = -moveSpeed * dirX;
	}
	//case 0x41:	//A
	if (GetAsyncKeyState(0x41)){
		xChange = moveSpeed * -dirY;
		yChange = moveSpeed * dirX;
	}
	//case 0x44:	//D
	if (GetAsyncKeyState(0x44)){
		xChange = moveSpeed * dirY;
		yChange = moveSpeed * -dirX;
	}
	//case 0x25:	//Left
	if (GetAsyncKeyState(0x25)){
		dirX = (oldDirX * cos(turnSpeed)) - (dirY * sin(turnSpeed));
		dirY = (dirY * cos(turnSpeed)) + (oldDirX * sin(turnSpeed));

		planeX = (planeX * cos(turnSpeed)) - (planeY * sin(turnSpeed));
		planeY = (planeY * cos(turnSpeed)) + (oldPlaneX * sin(turnSpeed));
	}
	//case 0x27:	//Right
	if (GetAsyncKeyState(0x27)){
		dirX = (oldDirX * cos(turnSpeed)) + (dirY * sin(turnSpeed));
		dirY = (dirY * cos(turnSpeed)) - (oldDirX * sin(turnSpeed));

		planeX = (planeX * cos(turnSpeed)) + (planeY * sin(turnSpeed));
		planeY = (planeY * cos(turnSpeed)) - (oldPlaneX * sin(turnSpeed));
	}
	if (GetAsyncKeyState(0x51)) {	//Q
		stretchX -= .1;
	}
	if (GetAsyncKeyState(0x45)) {	//E
		stretchX += .1;
	}

	//Check for collision and Move Character
	playerRadiusX = fabs(playerRadiusX);
	playerRadiusY = fabs(playerRadiusY);
	if (xChange < 0){
		playerRadiusX = -playerRadiusX;
	}
	if (yChange < 0){
		playerRadiusY = -playerRadiusY;
	}
	if (yChange != 0 && !grid[(int)floor(playerY + yChange + playerRadiusY)][(int)floor(playerX)] > 0){
		playerY += yChange;
	}
	if (xChange != 0 && !grid[(int)floor(playerY)][(int)floor(playerX + xChange + playerRadiusX)] > 0){
		playerX += xChange;
	}

	xChange = 0;
	yChange = 0;
}

void  Renderer::Render(Graphics* gfx){



	//Render
	gfx->BeginDraw();
	gfx->clearScreen(0, 0, 1);

	int mapOffsetX = 600;
	int mapOffsetY = 100;
	for (int y = 0; y < 6; y++){
		for (int x = 0; x < 6; x++){
			if (grid[y][x] > 0){
				gfx->drawRect( mapOffsetX + (x * 10), mapOffsetY + (y * 10), (mapOffsetX + (x * 10)) + 10, (mapOffsetY + (y * 10)) + 10, 0,0,0,1 );
			}
		}
	}
	gfx->drawCircle(mapOffsetX + (playerX * 10), mapOffsetY + (playerY * 10), 5, 255, 255, 255, 1);	//draw player on map

	//Raytracing:
	//rayX = playerX; //- fov;
	//rayY = playerY;
	rays = 0;
	while (rays < width){
		float raysNormalized = (rays - (width / 2)) / (width / 2);

		float rayDirX = dirX + (raysNormalized * planeX);
		float rayDirY = dirY + (raysNormalized * planeY);

		//DDA
		float sideDistX;
		float sideDistY;

		float deltaDistX = fabs(1 / rayDirX);
		float deltaDistY = fabs(1 / rayDirY);

		float stepX;
		float stepY;

		int mapX = floor(playerX);
		int mapY = floor(playerY);

		int side;

		if (rayDirX < 0){
			stepX = -1/width;//-.01f;
			sideDistX = (playerX - mapX) * deltaDistX;
		}
		else{
			stepX = 1 / width;
			sideDistX = (mapX + 1 - playerX) * deltaDistX;
		}
		if (rayDirY < 0){
			stepY = -1 / width;
			sideDistY = (playerY - mapY) * deltaDistY;
		}
		else{
			stepY = 1 / width;
			sideDistY = (mapY + 1 - playerY) * deltaDistY;
		}

		//end DDA


		float mapXfloat = playerX;
		float mapYfloat = playerY;

		while (grid[mapY][mapX] == 0){
			float oldRay[2] = { mapXfloat, mapYfloat };

			if (sideDistX < sideDistY){
				sideDistX += deltaDistX;
				mapXfloat += stepX;
				side = 0;
			}
			else{
				sideDistY += deltaDistY;
				mapYfloat += stepY;
				side = 1;
			}

			mapX = floor(mapXfloat);
			mapY = floor(mapYfloat);
			//gfx->drawLine(mapOffsetX + (oldRay[0] * 10), mapOffsetY + (oldRay[1] * 10), mapOffsetX + (mapX * 10), mapOffsetY + (mapY * 10), 255, 255, 255, 1);	//Debug Rays
		}


		
		

		gfx->drawLine(mapOffsetX + (playerX * 10), mapOffsetY + (playerY * 10), mapOffsetX + (mapXfloat * 10), mapOffsetY + (mapYfloat * 10), 255, 255, 255, 1);	//Debug Rays

		
		float perpWallDist;
		perpWallDist = sqrt(pow((mapXfloat - playerX),2)*stretchX + pow((mapYfloat -playerY) ,2)* stretchY);



		if (side == 0){
			//perpWallDist = ((mapX - playerX + (1 - stepX) / 2)  / rayDirX);
		}
		else{
			//perpWallDist = ((mapY - playerY + (1 - stepY) / 2) / rayDirY);
		}
		int lineHeight = static_cast<int>(height / perpWallDist);
		int drawStart = -lineHeight / 2 + height / 2;
		int drawEnd = lineHeight / 2 + height / 2;
		if (side == 0){
			gfx->drawLine(rays, drawStart, rays, drawEnd, 255, 255, 255, 1);
		}
		else{
			gfx->drawLine(rays, drawStart, rays, drawEnd, 255, 0, 0, 1);
		}

		rays += 1;
	}
	gfx->EndDraw();
}