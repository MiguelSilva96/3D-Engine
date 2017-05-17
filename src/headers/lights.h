#ifndef LIGHTS_H
#define LIGHTS_H

#include "transforms.h"

class Light {

	protected:
		float x, y, z; 
	public:
		Light(float a, float b, float c);
		virtual void placeLight(void) { };
};

class LightPoint : public Light {
	public:
		LightPoint(float a, float b, float c) : Light(a,b,c) {};
		void placeLight(void);
};

class SpotLight : public Light {
	public:
		SpotLight(float a, float b, float c) : Light(a,b,c) {};
};

class DirectionalLight : public Light {
	public:
		DirectionalLight(float a, float b, float c) : Light(a,b,c) {};
		void placeLight(void);
};

#endif