#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include "shapes.h"

/**
    Auxiliar function that returns a string that
    describes one vertex with the coordinates given
    as arguments.

    @param x Position on X axis
    @param y Position on Y axis
    @param z Position on Z axis
    @return String that describes the vertex
*/
std::string vertexString(float x, float y, float z) {
    std::stringstream strStream;
    strStream << x << " ";
    strStream << y << " ";
    strStream << z << "\n";
    return strStream.str();
}

std::vector<std::string> plane(float x, float y, float z, int div) {
    std::vector<std::string> v;
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            v.push_back(vertexString(x/2-x/div*(i+1), 
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, 
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i, 
                        y, -z/2+z/div*j));

            v.push_back(vertexString(x/2-x/div*(i+1), 
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*(i+1), 
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i, 
                        y, -z/2+z/div*(j+1)));
        }
    }
    return v;
}


std::vector<std::string> box(float x, float y, float z, int div) {
    std::vector<std::string> v;
    //top
    v = plane(x, y/2, z, div);
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            //base
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));

            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*(j+1)));
            //front
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            //back
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), -z/2));
            //right
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*i));

            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            //left
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));

            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
        }
    }
    return v;
}


std::vector<std::string> sphere(float radius, int slices, int stacks) {
    std::vector<std::string> v;
    double teta, fi;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            if (i != stacks - 1) {
                fi = (i + 1) * M_PI/stacks;
                teta = (j + 1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                fi = i * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
            }
            if (i != 0) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius*cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = (j + 1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                fi = (i + 1) * M_PI/stacks;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
            }
        }
    }
    return v;
}


std::vector<std::string> cylinder(float radB, float radT, float height, int slices, int stacks) {
    float alpha = (float) 2 * M_PI / slices;
    float h = (float) height / stacks;              //P1(0, 0, radB) é o ponto de referência, usado como ponto inicial
    float px = 0.0f, py = 0.0f, pz = (float) radB;
    float pOp1 = 0.0f;                              //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's no sentido de P1
    float tx, ty, tz, qx, qy, qz;
    float lvl;                                      //recebe o valor da altura em função das stacks percorridas
    std::vector<std::string> v;
    for(int j = 1; j <= slices; j++) {
        qx = px; qy = py; qz = pz;
        pOp1 = alpha * j;
        v.push_back(vertexString(px, 0.0f, pz));
        px = sin(pOp1) * radB;
        pz = cos(pOp1) * radB;
        v.push_back(vertexString(0.0f, 0.0f, 0.0f));
        v.push_back(vertexString(px, 0.0f, pz));
        //draw top
        tx = px; ty = py; tz = pz;
        for(int i = 1; i <= stacks; i++){
            lvl = radB - (float) (radB - radT) * i/stacks;
            v.push_back(vertexString(qx, qy, qz));
            v.push_back(vertexString(tx, ty, tz));
            v.push_back(vertexString(lvl*sin(alpha*(j-1)), h*i, lvl * cos(alpha * (j-1))));
            v.push_back(vertexString(lvl*sin(alpha*(j-1)), h*i, lvl * cos(alpha * (j-1))));
            v.push_back(vertexString(tx, ty, tz));
            v.push_back(vertexString(lvl * sin(pOp1), h*i, lvl * cos(pOp1)));
            tx = lvl * sin(pOp1);
            ty = h * i;
            tz = lvl * cos(pOp1);
            qx = lvl * sin(alpha*(j - 1));
            qy = h * i;
            qz = lvl * cos(alpha * (j - 1));
        }
    }
    if(radT > 0) {
        for(int j = 1; j <= slices; j++) {
            pOp1 = alpha * j;
            qx = sin(pOp1) * radT;
            qz = cos(pOp1) * radT;
            v.push_back(vertexString(qx, height, qz));
            pOp1 = alpha * (j - 1);
            qx = sin(pOp1) * radT;
            qz = cos(pOp1) * radT;
            v.push_back(vertexString(0.0f, height, 0.0f));
            v.push_back(vertexString(qx, height, qz));
        }
    }
    return v;
}


std::vector<std::string> torus(float inner, float outer, int sides, int rings) {
    float alpha = (float) 2 * M_PI / rings;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    std::vector<std::string> v;
    for (int i = 1; i <= rings; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)), 
                        r * sin(angle2), 
                        cos(angle1)*(outer - r*cos(angle2))));
        }   
    }
    return v;
}


/*
void drawRubi(int rb, int rt, int heightb, int heightt, int slices, int stacks){
    glColor3f(1.0, 0.0, 0.0);
    drawCone(rb, rt, heightt, slices, stacks);
    glRotatef(180, 1.0f, 0.0f, 0.0f);
    glColor3f(1, 1, 0);
    drawCone(rb, 0, heightb, slices,stacks);
}
*/