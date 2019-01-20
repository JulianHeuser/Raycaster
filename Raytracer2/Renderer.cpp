#include "Renderer.h"
#include "Graphics.h"
#include <math.h>
#include <string>

#define CAMWIDTH 500.0
#define CAMHEIGHT 500.0

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

void Renderer::Init() {


	cube.tris = {
		//Front face
		{ 20,10,20,		60,10,20,	20,10,60 },
		{ 60,10,60,		60,10,20,	20,10,60 },

		//Back face
		{ 20,50,20,		60,50,20,	20,50,60 },
		{ 60,50,60,		60,50,20,	20,50,60 },

		//Top face
		{ 20,50,60,		60,50,60,	20,10,60 },
		{ 60,10,60,		60,50,60,	20,10,60 },

		//Bottom face
		{ 20,50,20,		60,50,20,	20,10,20 },
		{ 60,10,20,		60,50,20,	20,10,20 },

		//Right face
		{ 60,10,60,		60,50,60,	60,50,20 },
		{ 60,10,60,		60,10,20,	60,50,20 },

		//Left face
		{ 20,10,60,		20,50,60,	20,50,20 },
		{ 20,10,60,		20,10,20,	20,50,20 },
	};
}

void Renderer::Update(){
	if (GetAsyncKeyState(0x41)) {	//A
		camPos.x -= 5;
	}
	if (GetAsyncKeyState(0x44)) {	//D
		camPos.x += 5;
	}
	if (GetAsyncKeyState(0x57)) {	//W
		camPos.y += 5;
	}
	if (GetAsyncKeyState(0x53)) {	//S
		camPos.y -= 5;
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		camRot.z -= .1;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		camRot.z += .1;
	}

	if (GetAsyncKeyState(VK_UP)) {
		camRot.x += .1;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		camRot.x -= .1;
	}

	//Z Rot
	matRotZ.m[0][0] = cos(camRot.z);
	matRotZ.m[0][1] = sin(camRot.z);
	matRotZ.m[1][0] = -sin(camRot.z);
	matRotZ.m[1][1] = cos(camRot.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//X Rot
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(camRot.x);
	matRotX.m[1][2] = sin(camRot.x);
	matRotX.m[2][1] = -sin(camRot.x);
	matRotX.m[2][2] = cos(camRot.x);
	matRotX.m[3][3] = 1;


	//Y Rot
	matRotY.m[0][0] = cos(camRot.y);
	matRotY.m[2][0] = sin(camRot.y);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sin(camRot.y);
	matRotY.m[2][2] = cos(camRot.y);
	matRotY.m[3][3] = 1;
}

void  Renderer::Render(Graphics* gfx){
	//Render
	gfx->BeginDraw();
	gfx->clearScreen(1, 1, 1);

	for (auto tri : cube.tris) {

		triangle triRot;
		triRot.p[0] = MultiplyMatrixValue(subtractPoints(tri.p[0], camPos), matRotZ);
		triRot.p[1] = MultiplyMatrixValue(subtractPoints(tri.p[1], camPos), matRotZ);
		triRot.p[2] = MultiplyMatrixValue(subtractPoints(tri.p[2], camPos), matRotZ);

		triRot.p[0] = MultiplyMatrixValue(subtractPoints(triRot.p[0], camPos), matRotX);
		triRot.p[1] = MultiplyMatrixValue(subtractPoints(triRot.p[1], camPos), matRotX);
		triRot.p[2] = MultiplyMatrixValue(subtractPoints(triRot.p[2], camPos), matRotX);

		triRot.p[0] = addPoints(triRot.p[0], camPos);
		triRot.p[1] = addPoints(triRot.p[1], camPos);
		triRot.p[2] = addPoints(triRot.p[2], camPos);

		//triRot.p[0] = MultiplyMatrixValue(triRot.p[0], matRotX);
		//triRot.p[1] = MultiplyMatrixValue(triRot.p[1], matRotX);
		//triRot.p[2] = MultiplyMatrixValue(triRot.p[2], matRotX);

		if (triRot.p[0].y > camPos.y) {
			vec3D newCamPos = camPos;
			newCamPos.x -= CAMWIDTH / 2;
			newCamPos.z -= CAMHEIGHT / 2;

			gfx->drawLine(triRot.p[0].x - newCamPos.x, triRot.p[0].z - newCamPos.z, triRot.p[1].x - newCamPos.x, triRot.p[1].z - newCamPos.z, 0, 0, 0, 1);
			gfx->drawLine(triRot.p[1].x - newCamPos.x, triRot.p[1].z - newCamPos.z, triRot.p[2].x - newCamPos.x, triRot.p[2].z - newCamPos.z, 0, 0, 0, 1);
			gfx->drawLine(triRot.p[2].x - newCamPos.x, triRot.p[2].z - newCamPos.z, triRot.p[0].x - newCamPos.x, triRot.p[0].z - newCamPos.z, 0, 0, 0, 1);
		}
	}


	gfx->EndDraw();
}

vec3D Renderer::MultiplyMatrixValue(vec3D i, mat4x4 &m) {
	vec3D o;

	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0) {
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
	return o;
}

vec3D Renderer::subtractPoints(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}


vec3D Renderer::addPoints(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}