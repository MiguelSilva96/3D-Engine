#include "lights.h"


//This light only represents type POINT and DIRECTIONAL
Light::Light(float a, float b, float c, int l) {
	GLfloat amb[4]  = {0.2, 0.2, 0.2, 1.0};
  	GLfloat diff[4] = {0.8, 0.8, 0.8, 1.0};
  	GLfloat spec[4] = {1.0, 1.0, 1.0, 1.0};
  	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
  	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	x = a; y = b; z = c;
	lightNum = l;
}

void LightPoint::placeLight(void) {
	float pos[4] = { x, y, z, 1 };
	glLightfv(GL_LIGHT0 + lightNum, GL_POSITION, pos);
}

void DirectionalLight::placeLight(void) {
	float pos[4] = { x, y, z, 0 };
	glLightfv(GL_LIGHT0 + lightNum, GL_POSITION, pos);
}

void SpotLight::placeLight(void) {
	float dir[4] = { x, y, z };
	glLightf(GL_LIGHT0 + lightNum, GL_SPOT_CUTOFF, cutOff);
	glLightfv(GL_LIGHT0 + lightNum, GL_SPOT_DIRECTION, dir);
}