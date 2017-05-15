#include "light.h"


//This light only represents type POINT for now
Light::Light(float a, float b, float c) {
	x = a; y = b; z = c;
}

void Light::placeLight(void) {
	float pos[4] = { x, y, z, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}