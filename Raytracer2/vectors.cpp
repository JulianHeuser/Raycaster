#include "vectors.h"
#include <math.h>

vec3D normalizeVector(vec3D vec) {
	float length = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	vec.x /= length;
	vec.y /= length;
	vec.z /= length;
	return vec;
}

vec3D MultiplyMatrixValue(vec3D i, mat4x4 &m) {
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

vec3D subtractVectors(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}


vec3D addVectors(vec3D a, vec3D b) {
	vec3D c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}

vec3D divideVector(vec3D vec, float div) {
	vec.x /= div;
	vec.y /= div;
	vec.z /= div;
	return vec;
}

vec3D multiplyVector(vec3D vec, float mult){
	vec.x *= mult;
	vec.y *= mult;
	vec.z *= mult;
	return vec;
}

float dot(vec3D vec1, vec3D vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

float dist(vec3D vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

bool vecIsZero(vec3D vec) {
	if (vec.x == 0 && vec.y == 0 && vec.z == 0) {
		return true;
	}
	return false;
}