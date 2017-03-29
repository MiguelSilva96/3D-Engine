#include "group.h"

Group::Group(std::vector<Vertex> v, std::vector<Transformation*> t, std::vector<Group> g) {
	vertexes   = v;
	transforms = t;
	subgroups  = g;
}

std::vector<Transformation*> Group::getTransformations(void) {
	return transforms;
}

std::vector<Vertex> Group::getVertexes(void) {
	return vertexes;
}

std::vector<Group> Group::getSubGroups(void) {
	return subgroups;
}