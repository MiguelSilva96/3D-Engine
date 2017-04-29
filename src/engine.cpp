/**
    Practical Assignment Computer Graphics
    engine.cpp
    Purpose: Display figures on the screen as instructed on input file

    @author Jose Silva, Joao Coelho, Luís Fernandes, Pedro Cunha
    @version 1.3
*/

#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "vertex.h"
#include "file.h"
#include "tinyxml2.h"
#include "xmlParser.h"
#include "transforms.h" //includes OpenGL too
#include <string.h>
#include <vector>
#include <regex>
#include <iostream>
#include <sstream>

using namespace std;

vector<Group> groups;
int modes[] = {GL_FILL, GL_LINE, GL_POINT};
int mode = 0;
float radius = 80;
float beta = 0, alfa = 0;
vector<float> vertexes;
GLuint buffers[2];

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio 
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);

    // Load Identity Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

float myRandom() {
    float r = ((double) rand() / (RAND_MAX));
    return r;
}

void bind(vector<Vertex> vert) {
    vector<Vertex>::iterator itVr;
    itVr = vert.begin();
    for(; itVr != vert.end(); ++itVr) {
        Vertex v = *itVr;
        vertexes.push_back(v.x);
        vertexes.push_back(v.y);
        vertexes.push_back(v.z);
    }

    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertexes.size() * sizeof(float), 
        &(vertexes[0]), 
        GL_STATIC_DRAW
    );
}

void draw() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

    vertexes.clear();
}

void renderRandom(Group g) {
    vector<pair<Color*,File*>> files;
    vector<pair<Color*,File*>>::iterator it;
    vector<Vertex> vert;

    srand(31457);
    int nr = 0;
    int out = g.outR;
    float in = g.inR;

    files = g.getVertexes();
    it = files.begin();
    for(; it != files.end(); ++it) {
        pair<Color*,File*> p = *it;
        File *f = p.second;
        vert = f -> getVertexes();
        while (nr < g.n) {

            float x = myRandom() * 2*out - out;
            float z = myRandom() * 2*out - out;
            float s = myRandom() * g.maxScale + g.minScale;
            float aux  = x*x + z*z;
            
            if (aux > in*in && aux < out*out) {
                glPushMatrix();
                p.first -> transform();
                glTranslatef(x, 0, z);
                glScalef(s, s, s);
                bind(vert);
                draw(    );
                glPopMatrix();
                nr++;
            }
        }
        nr = 0;
    }
}

void renderGroup(Group g) {
    vector<pair<Color*,File*>> files;
    vector<Group> subgroups;
    vector<Vertex> vert;
    vector<Transformation*> aux;
    vector<Transformation*> transforms;
    vector<Transformation*>::iterator itTr;
    vector<pair<Color*,File*>>::iterator it;
    vector<Group>::iterator itGr;

    glPushMatrix();
    
    if(g.n > 0) {
        renderRandom(g);
    } else {
        transforms = g.getTransformations();
        itTr = transforms.begin();
        for(; itTr != transforms.end(); ++itTr) {
            Transformation* t = *itTr;
            t -> transform();
        }

        files = g.getVertexes();
        it = files.begin();
        for(; it != files.end(); ++it) {
            pair<Color*,File*> p = *it;
            File *f = p.second;
            p.first -> transform();
            vert = f -> getVertexes();
            bind(vert);
            draw(    );
        }
    }
    subgroups = g.getSubGroups();
    itGr = subgroups.begin();
    for(; itGr != subgroups.end(); ++itGr) {
        Group gr = *itGr;
        renderGroup(gr);
    }
    
    glPopMatrix();
}


void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    float px = radius * cos(beta) * sin(alfa);
    float pz = radius * cos(beta) * cos(alfa);
    float py = radius * sin(beta);
    gluLookAt(px, py, pz, 
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT, modes[mode]);

    vector<Group>::iterator it;
    for(it = groups.begin(); it != groups.end(); ++it) {
        Group g = *it;
        renderGroup(g);
    }
    // End of frame
    glutSwapBuffers();
}

void manageEvents(int key_code, int x, int y) {
    switch(key_code) {
        case GLUT_KEY_RIGHT:
            alfa += 0.1;
            break;
        case GLUT_KEY_LEFT:
            alfa -= 0.1;
            break;
        case GLUT_KEY_UP:
            if(beta < 1.5f)
                beta += 0.1;
            break;
        case GLUT_KEY_DOWN:
            if(beta > -1.5f)
                beta -= 0.1;
            break;
        case GLUT_KEY_F1:
            mode = (mode+1)%3;
            break;
        case GLUT_KEY_PAGE_UP: 
            radius += 1.0f;
            break;
        case GLUT_KEY_PAGE_DOWN: 
            radius -= 1.0f;
            if (radius < 0.1f)
                radius = 0.1f;
    }
    glutPostRedisplay();
}

bool loadVertexes(const char* filename) {
    XmlParser *xmlparser = new XmlParser(filename);
    if(xmlparser -> readError())
        return false;
    while(xmlparser -> startNextGroup())
        groups.push_back(xmlparser -> getGroup());
    delete(xmlparser);
    return true;
}

void init() {

    // OpenGL settings 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);

}

int main(int argc, char **argv) {
    char *filename = (char *) malloc(64);

    // GLUT init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Engine");

    // init vertex vector
    if(argc > 1)
        filename = argv[1];
    else
        strcpy(filename, "../config.xml");
    if(!loadVertexes(filename)) {
        cout << "Error reading xml" << endl;
        return 1;
    }

    // Callback registration
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Registration of the keyboard callbacks
    glutSpecialFunc(manageEvents);

#ifndef __APPLE__   
    glewInit();
#endif  

    init();

    // GLUT's main loop
    glutMainLoop();
    free(filename);
    return 1;
}