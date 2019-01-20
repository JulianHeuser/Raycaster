#pragma once
#include "Graphics.h"
#include "shared.h"
#include <vector>;

class Renderer{
	//Variables for special 
	vec3D camPos = { 0 };
	vec3D camRot = { 0 };

	mat4x4 matRotX;
	mat4x4 matRotY;
	mat4x4 matRotZ;

	struct triangle {
		vec3D p[3];
	};

	struct mesh {
		vector<triangle> tris;
	};

	mesh cube;

public:
	void Init();
	void Render(Graphics* gfx);
	void Update(); 


	//Math stuff that I'm gonna do a lot
	vec3D MultiplyMatrixValue(vec3D i, mat4x4 &m);
	vec3D Renderer::subtractPoints(vec3D a, vec3D b);
	vec3D Renderer::addPoints(vec3D a, vec3D b);
};