#ifndef FILE_H
#define FILE_H

#include <vector>
#include "vertex.h"
#include "transforms.h"


class File {
    private:
    	int nVertex;
    	GLuint buffers[3];
    
    public:
    	File(vector<Vertex>, vector<Vertex>, vector<float>, char[]);
    	void draw(unsigned int);
		void drawPICK(int);
		char label[64];
};

#endif