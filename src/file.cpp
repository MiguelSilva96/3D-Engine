#include <file.h>

File::File(std::vector<Vertex> vs, std::vector<Vertex> ns) {
	vertexes = vs;
    normals  = ns;
	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertexes.size() * sizeof(float) * 3, 
        &(vertexes[0].x), 
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        normals.size() * sizeof(float) * 3, 
        &(normals[0].x),
        GL_STATIC_DRAW
    );
}

std::vector<Vertex> File::getVertexes(void) {
	return vertexes;
}


void File::draw(void) {
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glNormalPointer(GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size() * 3);
}

