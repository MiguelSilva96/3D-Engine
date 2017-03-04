/**
    Practical Assignment Computer Graphics
    generator.cpp
    Purpose: Generate vertexes to create geometric figures.

    @author Jose Silva, Joao Coelho, Luís Fernandes, Pedro Cunha
    @version 1.0
*/

#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
    Auxiliar function that returns a string that
    describes one vertex with the coordinates given
    as arguments.

    @param x Position on X axis
    @param y Position on Y axis
    @param z Position on Z axis
    @return String that describes the vertex
*/
string vertexString(float x, float y, float z) {
    stringstream strStream;
    strStream << x << " ";
    strStream << y << " ";
    strStream << z << "\n";
    return strStream.str();
}

/**
    Returns vertexes for a plane.
    
    @param x Length of the plane on X axis.
    @param y Position of the plane on Y axis.
    @param z Length of the plane on Z axis.
    @param div Number of divisions of the plane.
    @return
*/
vector<string> plane(float x, float y, float z, int div) {
    vector<string> v;
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


/**
    Returns vertexes for a box.
    
    @param x Length of the box on X axis.
    @param y Box's height.
    @param z Length of the box on Z axis.
    @param div Number of divisions of each face of the box.
    @return
*/
vector<string> box(float x, float y, float z, int div) {
    vector<string> v;
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            //top
            vector<string> aux = plane(x, y/2, z, div);
            v.insert(v.end(), aux.begin(), aux.end());
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

/**
    Returns vertexes for a sphere.

    @param radius Radius of the sphere.
    @param slices Number of slices.
    @param stacks Number of stacks.
    @returns
*/
vector<string> sphere(int radius, int slices, int stacks) {
    vector<string> v;
    float teta, fi;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            if (i != stacks - 1) {
                fi = i * M_PI / stacks;
                teta = j * 2 * M_PI / slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                fi = (i + 1) * M_PI / stacks;
                teta = (j + 1) * 2 * M_PI / slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = j * 2 * M_PI / slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
            }
            if (i != 0) {
                fi = i * M_PI / stacks;
                teta = j * 2 * M_PI / slices;
                v.push_back(vertexString(radius*cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = (j + 1) * 2 * M_PI / slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                fi = (i + 1) * M_PI / stacks;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
            }
        }
    }
    return v;
}


int main(int argc, char **argv) {
    vector<string> vertexes;
    //if(argc < 6)
    //  cout << "Input error\n";
    vertexes = box(2,3,3,10);
    vector<string>::iterator it;
    for(it = vertexes.begin(); it != vertexes.end(); ++it)
        cout << *it;
    //sphere(2,15,15);
}