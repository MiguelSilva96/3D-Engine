/**
    Practical Assignment Computer Graphics
    engine.cpp
    Purpose: Display figures on the screen as instructed on input file

    @author Jose Silva, Joao Coelho, Luís Fernandes, Pedro Cunha
    @version 1.3
*/


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
vector<Light*> lights;
int modes[] = {GL_FILL, GL_LINE, GL_POINT};
int w, h, mode = 0;
float radius = 80;
float beta = 0, alfa = 0;
float lx = 0.0f, ly = 0.0f, lz = 0.0f;
float px = 0.0f, py = 0.0f, pz = -100.0f;
float upx = 0.0f, upy = 1.0f, upz = 0.0f;
unsigned int picked = 0;
int code = 1;
char label[64];
map<int, char[64]> labels;

void changeSize(int ww, int hh) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
	w = ww;
	h = hh;
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


void renderRandom(Group g, bool picking) {
    vector<pair<Color**,File*>> files;
    vector<pair<Color**,File*>>::iterator it;
	char str[64];

    srand(31457);
    int nr = 0;
    int out = g.outR;
    float in = g.inR;

    files = g.getVertexes();
    it = files.begin();
    for(; it != files.end(); ++it) {
        pair<Color**,File*> p = *it;
        File *f = p.second;
        while (nr < g.n) {
            int i = 0;
            float x = myRandom() * 2*out - out;
            float z = myRandom() * 2*out - out;
            float s = myRandom() * g.maxScale + g.minScale;
            float aux  = x*x + z*z;
            
            if (aux > in*in && aux < out*out) {
                glPushMatrix();
                Color **c = p.first;
                while(c[i])
                    c[i++] -> transform();
                glTranslatef(x, 0, z);
                glScalef(s, s, s);
				if (picking) {
					if (strcmp(f->label,"")) {
						strcpy(labels[code], f->label);
						f->drawPICK(code);
					}
					else f->drawPICK(0);
				}
				else f->draw(c[0]->texID);
                glPopMatrix();
                nr++;
            }
        }
        nr = 0;
    }
}

void renderGroup(Group g, bool picking) {
    vector<pair<Color**,File*>> files;
    vector<Group> subgroups;
    vector<Transformation*> aux;
    vector<Transformation*> transforms;
    vector<Transformation*>::iterator itTr;
    vector<pair<Color**,File*>>::iterator it;
    vector<Group>::iterator itGr;
    int i;
	char str[64];

    glPushMatrix();
    
    if(g.n > 0) {
		renderRandom(g, picking);
		code++;
    } else {
        transforms = g.getTransformations();
        itTr = transforms.begin();
        for(; itTr != transforms.end(); ++itTr) {
            Transformation* t = *itTr;
            t -> transform();
        }

        files = g.getVertexes();
        it = files.begin();
        for(i = 0; it != files.end(); ++it) {
            pair<Color**,File*> p = *it;
            File *f = p.second;
            Color **c = p.first;

            while(c[i])
                c[i++] -> transform();
			if (picking) {
				if (strcmp(f->label, "")) {
					strcpy(labels[code], f->label);
					f->drawPICK(code);
					code++;
				}
				else f->drawPICK(0);
			}
			else f->draw(c[0]->texID);
        }
    }
    subgroups = g.getSubGroups();
    itGr = subgroups.begin();
    for(; itGr != subgroups.end(); ++itGr) {
        Group gr = *itGr;
        renderGroup(gr,picking);
    }

    glPopMatrix();
}

void renderText() {
	float orange[4] = { 0.8f, 0.4f , 0.4f,1.0f };
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	glMaterialfv(GL_FRONT, GL_EMISSION, orange);
	glRasterPos2d(10, 20);
	for (char *c = label; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void renderScene(void) {
    // clear buffers
    glClearColor(0.0f,0.0f,0.0f,0.0f);
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
    vector<Light*>::iterator itL;
    for(itL = lights.begin(); itL != lights.end(); ++itL) {
        Light *l = *itL;
        l -> placeLight();
    }

    vector<Group>::iterator it;
    for(it = groups.begin(); it != groups.end(); ++it) {
        Group g = *it;
        renderGroup(g, false);
    }
	renderText();
    // End of frame
    glutSwapBuffers();
}

unsigned char picking(int x, int y) {
	unsigned char res[4];
	code = 1;
	//Disable lighting and textures
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	lx = px + radius *sin(alfa) * cos(beta);
	ly = py + radius *sin(beta);
	lz = pz + radius *cos(alfa) * cos(beta);

	gluLookAt(px, py, pz,
		lx, ly, lz,
		upx, upy, upz);

	glDepthFunc(GL_LEQUAL);
	//Draw
	vector<Group>::iterator it;
	for (it = groups.begin(); it != groups.end(); ++it) {
		Group g = *it;
		renderGroup(g, true);
	}
	glDepthFunc(GL_LESS);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, res);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	return res[0];
}

void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_RIGHT_BUTTON) {
			picked = picking(xx, yy);
			if (picked) {
				sprintf(label, "%s", labels.find(picked)->second);
				//printf("%s\n",label);
			}
			else {
				sprintf(label, "Not labeled", picked);
				//printf("%s\n", label);
			}
			glutPostRedisplay();
		}
	}
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
    lights = xmlparser -> getLights();
    while(xmlparser -> startNextGroup())
        groups.push_back(xmlparser -> getGroup());
    delete(xmlparser);
    for(int i = 0; i < lights.size(); i++)
        glEnable(GL_LIGHT0 + i);
    return true;
}

void init() {

    // OpenGL settings 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

}

int main(int argc, char **argv) {
    char *filename = (char *) malloc(64);

    // GLUT init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Engine");

    // Callback registration
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // Registration of the keyboard callbacks
    glutSpecialFunc(manageEvents);
    glutKeyboardFunc(manageKeyboard);
	glutMouseFunc(processMouseButtons);

#ifndef __APPLE__   
    glewInit();
#endif  

    init();
    // init vertex vector
    if(argc > 1)
        filename = argv[1];
    else
        strcpy(filename, "../config.xml");
    if(!loadVertexes(filename)) {
        cout << "Error reading xml" << endl;
        return 1;
    }
    // GLUT's main loop
    glutMainLoop();
    free(filename);
    return 1;
}