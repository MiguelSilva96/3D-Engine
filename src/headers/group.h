#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "vertex.h"
#include "transforms.h"

class Group {
    private:
        std::vector<Vertex> vertexes;
        std::vector<Transformation*> transforms;
        std::vector<Group> subgroups;
    
    public:
        Group(std::vector<Vertex>, std::vector<Transformation*>, std::vector<Group>);
        std::vector<Transformation*> getTransformations(void);
        std::vector<Vertex> getVertexes(void);
        std::vector<Group> getSubGroups(void);
};

#endif