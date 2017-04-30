#include "transforms.h"

void TranslateCR::transform(void) {
	//do stuff for catmull-rom
	glTranslatef(x, y, z);
}


void RotateAnim::transform(void) {
	float now = glutGet(GLUT_ELAPSED_TIME);
	float timeMillis = time * 1000;
	float ang = fmod(now, timeMillis) / timeMillis * 360.0;

	glRotatef(ang, x, y, z);
}