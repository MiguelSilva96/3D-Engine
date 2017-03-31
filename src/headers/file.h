#ifndef FILE_H
#define FILE_H

#include <vector>
#include "vertex.h"


class File {
    private:
    	std::vector<Vertex> vertexes;
    
    public:
    	File(std::vector<Vertex>);
    	std::vector<Vertex> getVertexes(void);
};

#endif