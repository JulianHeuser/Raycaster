#include "Renderer.h"
#include "Graphics.h"
#include <string>

#define CAMWIDTH 500.0
#define CAMHEIGHT 500.0


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
		camPos.x -= .5f * sin(camRot.y + (PI / 2));
		camPos.z -= .5f * cos(camRot.y + (PI / 2));
	}
	if (GetAsyncKeyState(0x44)) {	//D
		camPos.x += .5f * sin(camRot.y + (PI / 2));
		camPos.z += .5f * cos(camRot.y + (PI / 2));
	}
	if (GetAsyncKeyState(0x57)) {	//W
		camPos.x += .5f * sin(camRot.y);
		camPos.z += .5f * cos(camRot.y);
		camPos.y += .5f * sin(camRot.x);
	}
	if (GetAsyncKeyState(0x53)) {	//S
		camPos.x -= .5f * sin(camRot.y);
		camPos.z -= .5f * cos(camRot.y);
		camPos.y -= .5f * sin(camRot.x);
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

		triangle triTranslated, triRot, triProjected, triFinal;

		vec3D lightDirRot = MultiplyMatrixValue(lightDir, matRotY);
		lightDirRot = MultiplyMatrixValue(lightDirRot, matRotX);

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

		//Calculate Normals
		vec3D line1, line2, normal;
		line1 = subtractVectors(triRot.p[1], triRot.p[0]);
		line2 = subtractVectors(triRot.p[2], triRot.p[0]);
		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;
		normalizeVector(&normal);

		float brightness = normal.x * lightDirRot.x + normal.y * lightDirRot.y + normal.z * lightDirRot.z;
		brightness = (brightness + 1)/2;

		//Dot product calculation to cull nonvisible faces
		if (normal.x * triRot.p[0].x + normal.y * triRot.p[0].y + normal.z * triRot.p[0].z < 0) {

			//Use projection matrix to get screen coordinates of points
			triProjected.p[0] = MultiplyMatrixValue(triRot.p[0], projection);
			triProjected.p[1] = MultiplyMatrixValue(triRot.p[1], projection);
			triProjected.p[2] = MultiplyMatrixValue(triRot.p[2], projection);

			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= WIDTH / 2.0f; triProjected.p[0].y *= WIDTH / 2.0f;
			triProjected.p[1].x *= WIDTH / 2.0f; triProjected.p[1].y *= WIDTH / 2.0f;
			triProjected.p[2].x *= WIDTH / 2.0f; triProjected.p[2].y *= WIDTH / 2.0f;

			//Draw the triangle
			triFinal = triProjected;
			gfx->drawTri(triFinal.p[0], triFinal.p[1], triFinal.p[2], brightness, brightness, brightness, 1);
		}
	}


	gfx->EndDraw();
}