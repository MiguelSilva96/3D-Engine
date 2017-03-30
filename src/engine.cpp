/**
    Practical Assignment Computer Graphics
    engine.cpp
    Purpose: Display figures on the screen as instructed on input file

    @author Jose Silva, Joao Coelho, Luís Fernandes, Pedro Cunha
    @version 1.0
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

void renderGroup(Group g) {
    vector<File*> files;
    vector<Group> subgroups;
    vector<Vertex> vert;
    vector<Transformation*> transforms;
    vector<Transformation*>::iterator itTr;
    vector<File*>::iterator it;
    vector<Vertex>::iterator itVr;
    vector<Group>::iterator itGr;
    
    glPushMatrix();
    
    transforms = g.getTransformations();
    itTr = transforms.begin();
    for(; itTr != transforms.end(); ++itTr) {
        Transformation* t = *itTr;
        t -> transform();
    }

    glBegin(GL_TRIANGLES);
    files = g.getVertexes();
    it = files.begin();
    for(; it != files.end(); ++it) {
        File *f = *it;
        vert = f -> getVertexes();
        itVr = vert.begin();
        for(; itVr != vert.end(); ++itVr) {
            Vertex v = *itVr;
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

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
        cout << "Error reading xml\n";
        return 1;
    }

    // Callback registration
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Registration of the keyboard callbacks
    glutSpecialFunc(manageEvents);
    
    // OpenGL settings 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    // GLUT's main loop
    glutMainLoop();
    free(filename);
    return 1;
}