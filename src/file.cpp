#include <file.h>
#include <iostream>

File::File(vector<Vertex> vs, vector<Vertex> ns, vector<float> ts) {
	vertexes = vs;
    normals  = ns;
    textureCoords = ts;
	glGenBuffers(3, buffers);
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
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        textureCoords.size() * sizeof(float), 
        &(textureCoords[0]),
        GL_STATIC_DRAW
    );
}

vector<Vertex> File::getVertexes(void) {
	return vertexes;
}


void File::draw(unsigned int texID) {
    glBindTexture(GL_TEXTURE_2D, texID);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size() * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    //reset to defaults
    float def[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    float defA[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float defD[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float defS[1] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, def);
    glMaterialfv(GL_FRONT, GL_SPECULAR, def);
    glMaterialfv(GL_FRONT, GL_AMBIENT, defA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, defD);
    glMaterialfv(GL_FRONT, GL_SHININESS, defS);
}

