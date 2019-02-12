#pragma once
#include "Graphics.h"
#include "shared.h"
#include "defines.h"
#include <vector>

class Renderer{
	//Variables for special 
	vec3D camPos = { 0 };
	vec3D camRot = { 0 };

	mat4x4 matRotX;
	mat4x4 matRotY;
	mat4x4 matRotZ;


	//Variables for projection
	float nearPlane = 0.1f;
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


	//Math stuff that I'm gonna do a lot
	vec3D MultiplyMatrixValue(vec3D i, mat4x4 &m);
	vec3D Renderer::subtractVectors(vec3D a, vec3D b);
	vec3D Renderer::addVectors(vec3D a, vec3D b);
};