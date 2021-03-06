#ifndef TRANSFORMS_H
#define TRANSFORMS_H


#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "vertex.h"
#include <math.h>
#include <vector>

#include <IL/il.h>


using namespace std;


class Transformation {
    public:
        const float x, y, z;
        Transformation(float a, float b, float c) : x(a), y(b), z(c) { }
        virtual void transform(void) { };
};

class Translate: public Transformation { 
	public:
		Translate(float a, float b, float c) : Transformation(a,b,c) { } 
		void transform(void) {
			glTranslatef(x, y, z);
		}
};

class TranslateCR: public Transformation { 
	private:
		vector<Vertex> points;
		GLuint buffers[3];
		vector<Vertex> vertexes;
		void getGlobalCatmullRomPoint(float gt, float *res, float *deriv);
		void buildRotMatrix(float *x, float *y, float *z, float *m);
		void cross(float *a, float *b, float *res);
		void normalize(float *a);
		float length(float *v);
		void multMatrixVector(float *m, float *v, float *res);
		float scalarProd(int len, float* vec1, float* vec2);
		void getCatmullRomPoint(float t, Vertex p0, Vertex p1, Vertex p2, Vertex p3, float *res, float *deriv);
		void renderCatmullRomCurve(void); 
	public:
		const float time;
		TranslateCR(float a, float b, float c, float d, vector<Vertex> e) : 
			Transformation(a,b,c), time(d), points(e) {
				float res[3], deriv[3];
				glGenBuffers(1, buffers);
				for(float i = 0; i < 1; i += 0.01) {
					getGlobalCatmullRomPoint(i, res, deriv);
					vertexes.push_back(Vertex(res[0], res[1], res[2]));
				}
				glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    			glBufferData(
        			GL_ARRAY_BUFFER, 
        			vertexes.size() * sizeof(float) * 3, 
        			&(vertexes[0].x), 
        			GL_STATIC_DRAW
    			);
			}
		void transform(void);
};

class Scale: public Transformation { 
	public:
		Scale(float a, float b, float c) : Transformation(a,b,c) { } 
		void transform(void) {
			glScalef(x, y, z);
		};
};

class Color: public Transformation {
	private:
		float *def;
	public:
		const int component;
		const unsigned int texID;
		Color(float a, float b, float c, int d, unsigned int e) : 
			Transformation(a,b,c), component(d), texID(e) {
			def = (float*)malloc(4*sizeof(float));
			def[0] = a;
			def[1] = b;
			def[2] = c;
			def[3] = 1;
		}
		Color(float a, int d, unsigned int e) : Transformation(a,a,a), component(d), texID(e) {
			def = (float*)malloc(sizeof(float));
			def[0] = a;
		} 
		void transform(void) {
			glMaterialfv(GL_FRONT, component, def);
		};
};

class Rotate: public Transformation {
	public:
		const float angle;
		Rotate(float a, float b, float c, float d) : 
						Transformation(a,b,c), angle(d) { }
		void transform(void) {
			glRotatef(angle, x, y, z);
		};
};

class RotateAnim: public Transformation {
	public:
		const float angle;
		const float time;
		RotateAnim(float a, float b, float c, float d, float e) : 
						Transformation(a,b,c), angle(d), time(e) { }
		void transform(void);
};

#endif