#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <utility>
#include "transforms.h"
#include "file.h"

using namespace std;

class Group {
    private:
        vector<pair<Color**,File*>> vertexes;
        vector<Transformation*> transforms;
        vector<Group> subgroups;
    
    public:
    	float n, outR, inR, minScale, maxScale;
        Group(vector<pair<Color**,File*>>, vector<Transformation*>, vector<Group>);
        vector<Transformation*> getTransformations(void);
        vector<pair<Color**,File*>> getVertexes(void);
        vector<Group> getSubGroups(void);

};

#endif