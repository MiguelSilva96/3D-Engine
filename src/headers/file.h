#ifndef FILE_H
#define FILE_H

#include <vector>
#include "vertex.h"
#include "transforms.h"


class File {
    private:
    	std::vector<Vertex> vertexes;
    	std::vector<Vertex> normals;
    	GLuint buffers[3];
    
    public:
    	File(std::vector<Vertex>, std::vector<Vertex>);
    	std::vector<Vertex> getVertexes(void);
    	void draw(void);
    	void prepare(void);
};

#endif