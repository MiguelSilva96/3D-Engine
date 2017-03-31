#include "group.h"

Group::Group(vector<pair<Color*,File*>> v, vector<Transformation*> t, vector<Group> g) {
	vertexes   = v;
	transforms = t;
	subgroups  = g;
	n = -1;
}

vector<Transformation*> Group::getTransformations(void) {
	return transforms;
}

vector<pair<Color*,File*>> Group::getVertexes(void) {
	return vertexes;
}

vector<Group> Group::getSubGroups(void) {
	return subgroups;
}
