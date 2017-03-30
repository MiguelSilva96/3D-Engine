#include <file.h>

File::File(std::vector<Vertex> vs) {
	vertexes = vs;
}

std::vector<Vertex> File::getVertexes(void) {
	return vertexes;
}