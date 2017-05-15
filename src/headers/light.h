#ifndef LIGHT_H
#define LIGHT_H

#include "transforms.h"

class Light {

	private:
		float x, y, z; 
	public:
		Light(float a, float b, float c);
		void placeLight(void);
};

#endif