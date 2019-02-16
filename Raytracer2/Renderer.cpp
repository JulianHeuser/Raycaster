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
		velocity.x -= .01f * sin(camRot.y + (PI / 2));
		velocity.z -= .01f * cos(camRot.y + (PI / 2));
	}
	if (GetAsyncKeyState(0x44)) {	//D
		velocity.x += .01f * sin(camRot.y + (PI / 2));
		velocity.z += .01f * cos(camRot.y + (PI / 2));
	}
	if (GetAsyncKeyState(0x57)) {	//W
		velocity.x += .01f * sin(camRot.y);
		velocity.z += .01f * cos(camRot.y);
		velocity.y += .01f * sin(camRot.x);
	}
	if (GetAsyncKeyState(0x53)) {	//S
		velocity.x -= .01f * sin(camRot.y);
		velocity.z -= .01f * cos(camRot.y);
		velocity.y -= .01f * sin(camRot.x);
	}
	velocity = divideVector(velocity, 1.1f);

	camPos = addVectors(camPos, velocity);

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

	vec3D lightDirRot = MultiplyMatrixValue(lightDir, matRotY);
	lightDirRot = MultiplyMatrixValue(lightDirRot, matRotX);

	for (auto tri : cube.tris) {

		triangle triTranslated, triRot, triProjected, triFinal;

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


		//Calculate Normals
		vec3D normal, line1, line2;
		line1 = subtractVectors(triRot.p[1], triRot.p[0]);
		line2 = subtractVectors(triRot.p[2], triRot.p[0]);
		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;
		normalizeVector(&normal);

		//Collision Detection
		float testRed = 0;

		//Face 1 normal
		vec3D nFace1, l1Face1, l2Face1;
		l1Face1 = subtractVectors(triRot.p[1], triRot.p[0]);
		l2Face1 = subtractVectors({ 0,0,0 }, triRot.p[0]);
		nFace1.x = l1Face1.y * l2Face1.z - l1Face1.z * l2Face1.y;
		nFace1.y = l1Face1.z * l2Face1.x - l1Face1.x * l2Face1.z;
		nFace1.z = l1Face1.x * l2Face1.y - l1Face1.y * l2Face1.x;
		normalizeVector(&nFace1);

		//Face 2 normal
		vec3D nFace2, l1Face2, l2Face2;
		l1Face2 = subtractVectors(triRot.p[2], triRot.p[1]);
		l2Face2 = subtractVectors({ 0,0,0 }, triRot.p[1]);
		nFace2.x = l1Face2.y * l2Face2.z - l1Face2.z * l2Face2.y;
		nFace2.y = l1Face2.z * l2Face2.x - l1Face2.x * l2Face2.z;
		nFace2.z = l1Face2.x * l2Face2.y - l1Face2.y * l2Face2.x;
		normalizeVector(&nFace2);

		//Face 3 normal
		vec3D nFace3, l1Face3, l2Face3;
		l1Face3 = subtractVectors(triRot.p[0], triRot.p[2]);
		l2Face3 = subtractVectors({ 0,0,0 }, triRot.p[2]);
		nFace3.x = l1Face3.y * l2Face3.z - l1Face3.z * l2Face3.y;
		nFace3.y = l1Face3.z * l2Face3.x - l1Face3.x * l2Face3.z;
		nFace3.z = l1Face3.x * l2Face3.y - l1Face3.y * l2Face3.x;
		normalizeVector(&nFace3);

		//Find out if the camera is on the projection plane for the tri
		if (dot(normal, nFace1) >= 0 && dot(normal, nFace2) >= 0 && dot(normal, nFace3) >= 0) {
			//Simplified component calculation (u dot v)/|v|
			float comp = -dot(triRot.p[0], normal) / 1;
			if (comp < 2) {
				testRed = 255;
			}
		}


		//Don't draw tris behind the camera
		if (triRot.p[0].z < nearPlane || triRot.p[1].z < nearPlane || triRot.p[2].z < nearPlane)
			continue;

		//Basic shading
		float brightness = normal.x * lightDirRot.x + normal.y * lightDirRot.y + normal.z * lightDirRot.z;
		brightness = (brightness + 1)/2;

		//Dot product calculation to cull nonvisible faces
		if (dot(normal, triRot.p[0]) < 0) {

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
			gfx->drawTri(triFinal.p[0], triFinal.p[1], triFinal.p[2], testRed, brightness/4, brightness/4, 1);
		}
	}


	gfx->EndDraw();
}