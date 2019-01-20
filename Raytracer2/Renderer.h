#pragma once
#include "Graphics.h"
#include "shared.h"
#include <vector>;

class Renderer{
	//Variables for special 
	float camPos[3] = { 0, 0, 0 };
	float camDir[3] = { 1, 0, 0 };

	mat4x4 matRotZ;
	mat4x4 matRotX;

	struct triangle {
		point p[3];
		//float p1[3];
		//float p2[3];
		//float p3[3];
	};

	struct mesh {
		vector<triangle> tris;
	};

	mesh cube;
	float cubeRot;

public:
	void Init();
	void Render(Graphics* gfx);
	void Update(); 

	point MultiplyMatrixValue(point i, mat4x4 &m);
};