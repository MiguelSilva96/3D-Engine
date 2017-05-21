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
    	File(vector<Vertex>, vector<Vertex>, vector<float>);
    	void draw(unsigned int);
    	void drawPICK(int);
};

#endif