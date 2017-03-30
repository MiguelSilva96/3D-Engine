#include "group.h"

Group::Group(std::vector<File*> v, std::vector<Transformation*> t, std::vector<Group> g) {
	vertexes   = v;
	transforms = t;
	subgroups  = g;
}

std::vector<Transformation*> Group::getTransformations(void) {
	return transforms;
}

std::vector<File*> Group::getVertexes(void) {
	return vertexes;
}

std::vector<Group> Group::getSubGroups(void) {
	return subgroups;
}
