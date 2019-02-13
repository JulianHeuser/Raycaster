#pragma once

struct mat4x4 {
	float m[4][4] = { 0 };
};

struct vec3D {
	float x, y, z;
};

void normalizeVector(vec3D *vec);
vec3D MultiplyMatrixValue(vec3D i, mat4x4 &m);

vec3D subtractVectors(vec3D a, vec3D b);
vec3D addVectors(vec3D a, vec3D b);

vec3D divideVector(vec3D vec, float div);
vec3D multiplyVector(vec3D vec, float mult);