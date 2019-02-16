#pragma once
#include "Graphics.h"
#include "vectors.h"
#include "defines.h"
#include <vector>

class Renderer{
	//Variables for special 
	vec3D camPos = { 0,0,-2 };
	vec3D camRot = { 0 };

	vec3D lightDir = { .4f,.6f,0 };

	vec3D velocity = { 0 };

	mat4x4 matRotX;
	mat4x4 matRotY;
	mat4x4 matRotZ;

	//Variables for projection
	float nearPlane = 0.0f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = HEIGHT / WIDTH;
	float fovRad = 1 / tanf(fov * .5f / 180 * PI);
	mat4x4 projection;

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
};