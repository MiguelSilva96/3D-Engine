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
	public:
		SpotLight(float a, float b, float c, float d, int l) : Light(a,b,c,l), cutOff(d) {};
		void placeLight(void);
};

class DirectionalLight : public Light {
	public:
		DirectionalLight(float a, float b, float c, int l) : Light(a,b,c,l) {};
		void placeLight(void);
};

#endif