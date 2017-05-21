#ifndef LIGHTS_H
#define LIGHTS_H

#include "transforms.h"

class Light {

	protected:
		float x, y, z;
		int lightNum;
	public:
		Light(float a, float b, float c, int l);
		virtual void placeLight(void) { };
};

class LightPoint : public Light {
	public:
		LightPoint(float a, float b, float c, int l) : Light(a,b,c,l) {};
		void placeLight(void);
};

class SpotLight : public Light {
	private:
		float cutOff;
		float dx, dy, dz;
	public:
		SpotLight(float a, float b, float c, float d, float e, float f, float g, int l) : 
		Light(a,b,c,l), cutOff(d), dx(e), dy(f), dz(g) {};
		void placeLight(void);
};

class DirectionalLight : public Light {
	public:
		DirectionalLight(float a, float b, float c, int l) : Light(a,b,c,l) {};
		void placeLight(void);
};

#endif