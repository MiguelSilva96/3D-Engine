#include <file.h>
#include <iostream>

File::File(vector<Vertex> vs, vector<Vertex> ns, vector<float> ts) {
    void *norm = nullptr;
    void *txtr = nullptr;
    if(ns.size() > 0)
        norm = &(ns[0].x);
    if(ts.size() > 0)
        txtr = &(ts[0]);
    nVertex = vs.size() * 3;
	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vs.size() * sizeof(float) * 3, 
        &(vs[0].x), 
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        ns.size() * sizeof(float) * 3, 
        norm,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        ts.size() * sizeof(float), 
        txtr,
        GL_STATIC_DRAW
    );
}


void File::draw(unsigned int texID) {
    glBindTexture(GL_TEXTURE_2D, texID);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, nVertex);
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

