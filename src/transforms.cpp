#include "transforms.h"


void TranslateCR::transform(void) {
  	float res[3], deriv[3];
  	float now = glutGet(GLUT_ELAPSED_TIME);
  	float timeMillis = time * 1000;
  	float t = fmod(now, timeMillis) / timeMillis;

  	renderCatmullRomCurve();

	getGlobalCatmullRomPoint(t, res, deriv);

  	glTranslatef(res[0], res[1], res[2]);
  	float up[3] = { 0, 1, 0 };
  	float z[3];
  	cross(deriv,up,z);
	cross(z, deriv, up);
	normalize(deriv);
	normalize(up);
	normalize(z);
	float m[4][4] = { { deriv[0],  deriv[1], deriv[2], 0 },
	{ up[0], up[1],  up[2], 0 },
	{ z[0],  z[1],  z[2],  0 },
	{ 0.0f,  0.0f,  0.0f,  1.0f } };

	glMultMatrixf((float*) m);

}


void RotateAnim::transform(void) {
	float now = glutGet(GLUT_ELAPSED_TIME);
	float timeMillis = time * 1000;
	float ang = fmod(now, timeMillis) / timeMillis * 360.0;

	glRotatef(ang, x, y, z);
}


void TranslateCR::buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void TranslateCR::cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void TranslateCR::normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


float TranslateCR::length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void TranslateCR::multMatrixVector(float *m, float *v, float *res) {

	for(int j = 0; j < 4; ++j) {
		res[j] = 0;
		for(int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

float TranslateCR::scalarProd(int len, float* vec1, float* vec2) {
	float value = 0;
	for (int i = 0; i < len; i++) {
		value += vec1[i] * vec2[i];
	}
	return value;
}


void TranslateCR::getCatmullRomPoint(float t, Vertex p0, Vertex p1, Vertex p2, Vertex p3, float *res, float *deriv) {
	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
					  { 1.0f, -2.5f,  2.0f, -0.5f},
					  {-0.5f,  0.0f,  0.5f,  0.0f},
					  { 0.0f,  1.0f,  0.0f,  0.0f} };

	// reset res
	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

	// Compute A = M * P
	float a[3][4];
	float px[4] = { p0.x, p1.x, p2.x, p3.x };
	float py[4] = { p0.y, p1.y, p2.y, p3.y };
	float pz[4] = { p0.z, p1.z, p2.z, p3.z };

	multMatrixVector((float *)m, px, a[0]);
	multMatrixVector((float *)m, py, a[1]);
	multMatrixVector((float *)m, pz, a[2]);
	// Compute point res = T *A
	float vec_t[4] = { t*t*t, t*t, t, 1 };
	res[0] = scalarProd(4, vec_t, a[0]);
	res[1] = scalarProd(4, vec_t, a[1]);
	res[2] = scalarProd(4, vec_t, a[2]);
	// compute deriv = T' * A
	float dev_t[4] = { 3 * t*t, 2 * t, 1, 0 };
	deriv[0] = scalarProd(4, dev_t, a[0]);
	deriv[1] = scalarProd(4, dev_t, a[1]);
	deriv[2] = scalarProd(4, dev_t, a[2]);
}


// given  global t, returns the point in the curve
void TranslateCR::getGlobalCatmullRomPoint(float gt, float *res, float *deriv) {
	int pointCount = points.size();
	
	float t = gt * pointCount; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + pointCount-1)%pointCount;	
	indices[1] = (indices[0]+1)%pointCount;
	indices[2] = (indices[1]+1)%pointCount; 
	indices[3] = (indices[2]+1)%pointCount;

	getCatmullRomPoint(
		t, points.at(indices[0]), 
		points.at(indices[1]), 
		points.at(indices[2]), 
		points.at(indices[3]), 
		res, deriv
	);
}

void TranslateCR::renderCatmullRomCurve(void) {
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float def[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, white);
    glDrawArrays(GL_LINE_LOOP, 0, vertexes.size());
    glMaterialfv(GL_FRONT, GL_EMISSION, def);
}