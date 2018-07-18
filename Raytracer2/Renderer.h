#pragma once

#include "Gamelevel.h"

class Renderer : public GameLevel{
	//Variables needed for basic rendering
	float playerX = 3.0;
	float playerY = 3.0;
	float playerRadiusX = 0.25;
	float playerRadiusY = 0.25;

	float planeX = 0;
	float planeY = .66f;

	float fov = 1;
	float moveSpeed = 0.1;
	float turnSpeed = .1;

	float dirX = -1;
	float dirY = 0;

	float xChange = 0;
	float yChange = 0;

	float rays = 0; //How many rays have been drawn

	float width = 500;
	int height = 500;

	int grid[6][6];

public:
	static int currentLevel;
	static bool Loading;
	void Load(int level) override;
	void Unload(int level) override;
	void Update() override;
	void Render(Graphics* gfx) override;
};