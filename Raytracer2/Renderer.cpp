#include "Renderer.h"
#include "Graphics.h"
#include <math.h>
#include <string>

#define CAMWIDTH 500;
#define CAMHEIGHT 500;

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
		camPos[0] -= 1;
	}
	if (GetAsyncKeyState(0x44)) {	//D
		camPos[0] += 1;
	}

	if (GetAsyncKeyState(0x25)) {	//Left
		cubeRot -= 5;
	}
	if (GetAsyncKeyState(0x27)) {	//Right
		cubeRot += 5;
	}


	//Z Rot
	matRotZ.m[0][0] = cos(cubeRot);
	matRotZ.m[0][1] = sin(cubeRot);
	matRotZ.m[1][0] = -sin(cubeRot);
	matRotZ.m[1][1] = cos(cubeRot);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//X Rot
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(cubeRot * .5f);
	matRotX.m[1][2] = sin(cubeRot * .5f);
	matRotX.m[2][1] = -sin(cubeRot * .5f);
	matRotX.m[2][2] = cos(cubeRot * .5f);
	matRotX.m[3][3] = 1;
}

void  Renderer::Render(Graphics* gfx){
	//Render
	gfx->BeginDraw();
	gfx->clearScreen(0, 0, 1);

	for (auto tri : cube.tris) {

		triangle triRot;
		triRot.p[0] = MultiplyMatrixValue(tri.p[0], matRotZ);
		triRot.p[1] = MultiplyMatrixValue(tri.p[1], matRotZ);
		triRot.p[2] = MultiplyMatrixValue(tri.p[2], matRotZ);


		triRot.p[0] = MultiplyMatrixValue(triRot.p[0], matRotX);
		triRot.p[1] = MultiplyMatrixValue(triRot.p[1], matRotX);
		triRot.p[2] = MultiplyMatrixValue(triRot.p[2], matRotX);

		gfx->drawLine(triRot.p[0].x - camPos[0], triRot.p[0].y - camPos[1], triRot.p[1].x - camPos[0], triRot.p[1].y - camPos[1], 0,0,0,1);
		gfx->drawLine(triRot.p[1].x - camPos[0], triRot.p[1].y - camPos[1], triRot.p[2].x - camPos[0], triRot.p[2].y - camPos[1], 0, 0, 0, 1);
		gfx->drawLine(triRot.p[2].x - camPos[0], triRot.p[2].y - camPos[1], triRot.p[0].x - camPos[0], triRot.p[0].y - camPos[1], 0, 0, 0, 1);
	}


	gfx->EndDraw();
}

point Renderer::MultiplyMatrixValue(point i, mat4x4 &m) {
	point o;

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