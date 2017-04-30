#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "vertex.h"
#include <math.h>
#include <vector>

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
	public:
		const float time;
		TranslateCR(float a, float b, float c, float d, vector<Vertex> e) : 
			Transformation(a,b,c), time(d), points(e) { }
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
	public:
		Color(float a, float b, float c) : Transformation(a,b,c) { } 
		void transform(void) {
			glColor3f(x, y, z);
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