#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class Transformation {
    public:
        const float x, y, z;
        Transformation(float a, float b, float c) : x(a), y(b), z(c) { }
        virtual void transform(void){ };
};

class Translate: public Transformation { 
	public:
		Translate(float a, float b, float c) : Transformation(a,b,c) { } 
		void transform(void) {
			glTranslatef(x, y, z);
		}
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
			glColor3ub(x, y, z);
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

#endif