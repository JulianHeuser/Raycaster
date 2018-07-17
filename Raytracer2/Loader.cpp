#include "Loader.h"
#include "GameLoader.h"
#include "Graphics.h"
#include <math.h>
#include "Levels.h"
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

void  Loader::Load(int level){
	//Levels
	switch (level)
	{
	case(1) :
		for (int i = 0; i < 6; i++){
			for (int o = 0; o < 6; o++){
				grid[i][o] = level1[i][o];
			}
		}
		break;
	case(2) :
		for (int i = 0; i < 6; i++){
			for (int o = 0; o < 6; o++){
				grid[i][o] = level2[i][o];
			}
		}
		break;
	default:
		break;
	}
}

void  Loader::Unload(){}

void Loader::Update(){

}

void  Loader::Render(Graphics* gfx){

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


	//Move Character
	if (xChange < 0){
		playerRadius = -playerRadius;
	}
	else{
		playerRadius = abs(playerRadius);
	}
	if (xChange != 0 && grid[(int)floor(playerY)][(int)floor(playerX + xChange + playerRadius)] == 0){
		playerX += xChange;
		xChange = 0;
	}

	if (yChange < 0){
		playerRadius = -playerRadius;
	}
	else{
		playerRadius = abs(playerRadius);
	}
	if (yChange != 0 && grid[(int)floor(playerY + yChange + playerRadius)][(int)floor(playerX)] == 0){
		playerY += yChange;
		yChange = 0;
	}

	//Render
	gfx->BeginDraw();
	gfx->clearScreen(0, 0, 1);

	int mapOffsetX = 600;
	int mapOffsetY = 100;
	for (int y = 0; y < 6; y++){
		for (int x = 0; x < 6; x++){
			if (grid[y][x] > 0){
				//Rectangle(hdc, mapOffsetX + (x * 10), mapOffsetY + (y * 10), (mapOffsetX + (x * 10)) + 10, (mapOffsetY + (y * 10)) + 10);
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

		float deltaDistX = abs(1 / rayDirX);
		float deltaDistY = abs(1 / rayDirY);

		int stepX;
		int stepY;

		int mapX = static_cast<int>(floor(playerX));
		int mapY = static_cast<int>(floor(playerY));

		int side;

		if (rayDirX < 0){
			stepX = -1;
			sideDistX = (playerX - mapX) * deltaDistX;
		}
		else{
			stepX = 1;
			sideDistX = (mapX + 1 - playerX) * deltaDistX;
		}
		if (rayDirY < 0){
			stepY = -1;
			sideDistY = (playerY - mapY) * deltaDistY;
		}
		else{
			stepY = 1;
			sideDistY = (mapY + 1 - playerY) * deltaDistY;
		}

		//end DDA

		while (grid[mapY][mapX] == 0){
			float oldRay[2] = { mapX, mapY };

			if (sideDistX < sideDistY){
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			gfx->drawLine(mapOffsetX + (oldRay[0] * 10), mapOffsetY + (oldRay[1] * 10), mapOffsetX + (mapX * 10), mapOffsetY + (mapY * 10), 255, 255, 255, 1);	//Debug Rays
		}
		//float dist = sqrt(pow(abs(rayX - oldRay[0]), 2) + pow(abs(rayY - oldRay[1]), 2));
		float dist = sqrt(pow(abs(mapX - playerX), 2) + pow(abs(mapY - playerY), 2));
		float size = (dist / 6)*height;

		float perpWallDist;
		//drawLine(rays, size/2, rays, height - (size/2), hWnd);
		if (side == 0){
			perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
		}
		else{
			perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
		}
		int lineHeight = static_cast<int>(height / perpWallDist);
		int drawStart = -lineHeight / 2 + height / 2;
		int drawEnd = lineHeight / 2 + height / 2;
		if (side == 0){
			gfx->drawLine(rays, drawStart, rays, drawEnd, 255, 255, 255, 1);
		}
		else{
			gfx->drawLine(rays, drawStart, rays, drawEnd, 100, 100, 100, 1);
		}


		//create_line(rays, size / 2, rays, height - (size / 2));
		//rayX += interval;
		rays += 1;


	}
	gfx->EndDraw();
}