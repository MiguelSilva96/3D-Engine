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
float lx = 0.0f, ly = 0.0f, lz = 0.0f;
float px = 0.0f, py = 0.0f, pz = -100.0f;
float upx = 0.0f, upy = 1.0f, upz = 0.0f;

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

void cross(float *a, float *b, float *res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

float myRandom() {
    float r = ((double) rand() / (RAND_MAX));
    return r;
}


void renderRandom(Group g) {
    vector<pair<Color*,File*>> files;
    vector<pair<Color*,File*>>::iterator it;

    srand(31457);
    int nr = 0;
    int out = g.outR;
    float in = g.inR;

    files = g.getVertexes();
    it = files.begin();
    for(; it != files.end(); ++it) {
        pair<Color*,File*> p = *it;
        File *f = p.second;
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
                f -> prepare();
                f -> draw();
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
            f -> prepare();
            f -> draw();
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
    lx = px + radius *sin(alfa) * cos(beta);
    ly = py + radius *sin(beta);
    lz = pz + radius *cos(alfa) * cos(beta);
    gluLookAt(px, py, pz, 
              lx, ly, lz,
              upx, upy, upz);

    glPolygonMode(GL_FRONT, modes[mode]);

    vector<Group>::iterator it;
    for(it = groups.begin(); it != groups.end(); ++it) {
        Group g = *it;
        renderGroup(g);
    }
    // End of frame
    glutSwapBuffers();
}

void manageKeyboard(unsigned char key_code, int x, int y) {
    int step = 10;
    float d[3];
    d[0] = lx - px;
    d[1] = ly - py;
    d[2] = lz - pz;
    normalize(d);
    float up[3] = { upx, upy, upz };
    float r[3];
    cross(d, up, r);

    switch (key_code) {
        case 'w':
        case 'W':
            px += step * d[0];
            py += step * d[1];
            pz += step * d[2];
            lx += step * d[0];
            ly += step * d[1];
            lz += step * d[2];
            break;
        case 's':
        case 'S':
            px -= step * d[0];
            py -= step * d[1];
            pz -= step * d[2];
            lx -= step * d[0];
            ly -= step * d[1];
            lz -= step * d[2];
            break;
        case 'a':
        case 'A':
            px -= step * r[0];
            py -= step * r[1];
            pz -= step * r[2];
            lx -= step * r[0];
            ly -= step * r[1];
            lz -= step * r[2];
            break;
        case 'd':
        case 'D':
            px += step * r[0];
            py += step * r[1];
            pz += step * r[2];
            lx += step * r[0];
            ly += step * r[1];
            lz += step * r[2];
            break;
    }
    glutPostRedisplay();
}

void manageEvents(int key_code, int x, int y) {
    switch(key_code) {
        case GLUT_KEY_RIGHT:
            alfa -= 0.1;
            break;
        case GLUT_KEY_LEFT:
            alfa += 0.1;
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
    vector<Group>::iterator it;
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
    glutKeyboardFunc(manageKeyboard);

#ifndef __APPLE__   
    glewInit();
#endif  

    init();

    // GLUT's main loop
    glutMainLoop();
    free(filename);
    return 1;
}