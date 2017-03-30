#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "transforms.h"
#include "file.h"

class Group {
    private:
        std::vector<File*> vertexes;
        std::vector<Transformation*> transforms;
        std::vector<Group> subgroups;
    
    public:
        Group(std::vector<File*>, std::vector<Transformation*>, std::vector<Group>);
        std::vector<Transformation*> getTransformations(void);
        std::vector<File*> getVertexes(void);
        std::vector<Group> getSubGroups(void);

};

#endif