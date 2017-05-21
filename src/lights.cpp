#include "lights.h"


Light::Light(float a, float b, float c, int l) {
	GLfloat amb[4]  = {0.2, 0.2, 0.2, 1.0};
  	GLfloat diff[4] = {0.8, 0.8, 0.8, 1.0};
  	GLfloat spec[4] = {1.0, 1.0, 1.0, 1.0};
  	glLightfv(GL_LIGHT0 + l, GL_AMBIENT, amb);
  	glLightfv(GL_LIGHT0 + l, GL_DIFFUSE, diff);
  	glLightfv(GL_LIGHT0 + l, GL_SPECULAR, spec);
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
	float dir[4] = { dx, dy, dz };
	float pos[4] = { x, y, z, 1 };
	glLightfv(GL_LIGHT0 + lightNum, GL_POSITION, pos);
	glLightfv(GL_LIGHT0 + lightNum, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT0 + lightNum, GL_SPOT_CUTOFF, cutOff);
}