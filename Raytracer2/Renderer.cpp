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
		{ 0,0,0,	0,1,0,	1,1,0 },
		{ 0,0,0,	1,1,0,	1,0,0 },

		//Back face
		{ 1,0,0,	1,1,0,	1,1,1 },
		{ 1,0,0,	1,1,1,	1,0,1 },

		//Top face
		{ 1,0,1,	1,1,1,	0,1,1 },
		{ 1,0,1,	0,1,1,	0,0,1 },

		//Bottom face
		{ 0,0,1,	0,1,1,	0,1,0 },
		{ 0,0,1,	0,1,0,	0,0,0 },

		//Right face
		{ 0,1,0,	0,1,1,	1,1,1 },
		{ 0,1,0,	1,1,1,	1,1,0 },

		//Left face
		{ 1,0,1,	0,0,1,	0,0,0 },
		{ 1,0,1,	0,0,0,	1,0,0 },
	};
}

void Renderer::Update(){
	if (GetAsyncKeyState(0x41)) {	//A
		camPos.x -= .5f * sin(camRot.y + (90*(PI/180)));
		camPos.z -= .5f * cos(camRot.y + (90 * (PI / 180)));
	}
	if (GetAsyncKeyState(0x44)) {	//D
		camPos.x += .5f * sin(camRot.y + (90 * (PI / 180)));
		camPos.z += .5f * cos(camRot.y + (90 * (PI / 180)));
	}
	if (GetAsyncKeyState(0x57)) {	//W
		camPos.x += .5f * sin(camRot.y);
		camPos.z += .5f * cos(camRot.y);
	}
	if (GetAsyncKeyState(0x53)) {	//S
		camPos.x -= .5f * sin(camRot.y);
		camPos.z -= .5f * cos(camRot.y);
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		camRot.y -= .1f;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		camRot.y += .1f;
	}

	if (GetAsyncKeyState(VK_UP)) {
		camRot.x -= .1f;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		camRot.x += .1f;
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
	matRotY.m[0][0] = cos(-camRot.y);
	matRotY.m[2][0] = sin(-camRot.y);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sin(-camRot.y);
	matRotY.m[2][2] = cos(-camRot.y);
	matRotY.m[3][3] = 1;

	//Projection matrix
	projection.m[0][0] = aspectRatio * fovRad;
	projection.m[1][1] = fovRad;
	projection.m[2][2] = farPlane / (farPlane - nearPlane);
	projection.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projection.m[2][3] = 1.0f;
	projection.m[3][3] = 0.0f;
}

void  Renderer::Render(Graphics* gfx){
	//Render
	gfx->BeginDraw();
	gfx->clearScreen(1, 1, 1);

	for (auto tri : cube.tris) {

		triangle triRot;
		triangle triTranslated;
		triangle triProjected;
		triangle triFinal;

		//Translate points according to camera location
		triTranslated.p[0] = subtractVectors(tri.p[0], camPos);
		triTranslated.p[1] = subtractVectors(tri.p[1], camPos);
		triTranslated.p[2] = subtractVectors(tri.p[2], camPos);

		//Rotate points around camera based on camera rotation
		triRot.p[0] = MultiplyMatrixValue(triTranslated.p[0], matRotY);
		triRot.p[1] = MultiplyMatrixValue(triTranslated.p[1], matRotY);
		triRot.p[2] = MultiplyMatrixValue(triTranslated.p[2], matRotY);

		triRot.p[0] = MultiplyMatrixValue(triRot.p[0], matRotX);
		triRot.p[1] = MultiplyMatrixValue(triRot.p[1], matRotX);
		triRot.p[2] = MultiplyMatrixValue(triRot.p[2], matRotX);

		//Don't draw tris behind the camera
		if (triRot.p[0].z < nearPlane || triRot.p[1].z < nearPlane || triRot.p[2].z < nearPlane)
			continue;

		//Use projection matrix to get screen coordinates of points
		triProjected.p[0] = MultiplyMatrixValue(triRot.p[0], projection);
		triProjected.p[1] = MultiplyMatrixValue(triRot.p[1], projection);
		triProjected.p[2] = MultiplyMatrixValue(triRot.p[2], projection);

		triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

		triProjected.p[0].x *= 250.0f; triProjected.p[0].y *= 250.0f;
		triProjected.p[1].x *= 250.0f; triProjected.p[1].y *= 250.0f;
		triProjected.p[2].x *= 250.0f; triProjected.p[2].y *= 250.0f;

		//Draw the triangle
		triFinal = triProjected;
		gfx->drawLine(triFinal.p[0].x, triFinal.p[0].y, triFinal.p[1].x, triFinal.p[1].y, 0, 0, 0, 1);
		gfx->drawLine(triFinal.p[1].x, triFinal.p[1].y, triFinal.p[2].x, triFinal.p[2].y, 0, 0, 0, 1);
		gfx->drawLine(triFinal.p[2].x, triFinal.p[2].y, triFinal.p[0].x, triFinal.p[0].y, 0, 0, 0, 1);
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

vec3D Renderer::subtractVectors(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}


vec3D Renderer::addVectors(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}