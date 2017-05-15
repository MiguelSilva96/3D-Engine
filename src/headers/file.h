#ifndef FILE_H
#define FILE_H

#include <vector>
#include "vertex.h"
#include "transforms.h"


class File {
    private:
    	vector<Vertex> vertexes;
    	vector<Vertex> normals;
    	vector<float> textureCoords;
    	GLuint buffers[3];
    
    public:
    	File(vector<Vertex>, vector<Vertex>, vector<float>);
    	vector<Vertex> getVertexes(void);
    	void draw(unsigned int);
};

#endif