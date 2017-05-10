/**
    Practical Assignment Computer Graphics
    generator.cpp
    Purpose: Generate vertexes to create geometric figures.

    @author Jose Silva, Joao Coelho, Luís Fernandes, Pedro Cunha
    @version 1.0
*/

#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include "shapes.h"


/**
    Verifies if input has the required form.

    @param input Input strings.
    @return Boolean with the result.
*/

bool verifyInput(char **input) {
    regex ints("[-+]?[0-9]+");
    regex floats("[-+]?[0-9]+\\.?[0-9]*");
    char *shape = input[1];
    bool result = false;
    if(!strcmp(shape, "plane"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], ints)
                && (input[5] != NULL);

    else if(!strcmp(shape, "box"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], floats)
                && regex_match(input[5], ints)
                && (input[6] != NULL);

    else if(!strcmp(shape, "sphere"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], ints)
                && regex_match(input[4], ints)
                && (input[5] != NULL);

    else if(!strcmp(shape, "cylinder"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], floats)
                && regex_match(input[5], ints)
                && regex_match(input[6], ints)
                && (input[7] != NULL);

    else if(!strcmp(shape, "cone") || !strcmp(shape, "torus"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], ints)
                && regex_match(input[5], ints)
                && (input[6] != NULL);

    else if(!strcmp(shape, "ruby"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], floats)
                && regex_match(input[5], floats)
                && regex_match(input[6], ints)
                && regex_match(input[7], ints)
                && (input[8] != NULL);
    else if(!strcmp(shape, "crown"))
        result = regex_match(input[2], floats)
                && regex_match(input[3], floats)
                && regex_match(input[4], ints)
                && (input[5] != NULL);
    else if(!strcmp(shape, "bezier")) {
        result = regex_match(input[3], ints);
    }
    return result;
}

/**
    Takes input to create vertexes as instructed.

    @param input Input strings.
    @return Vector with all strings of the vertexes.
*/
vector<string>* getVertexes(char **input) {
    char *shape = input[1];
    char *file;
    float x, y, z, radius, height;
    float r1, r2, h1, h2;
    int div, slices, stacks, sides, rings;
    vector<string> *vertexes;
    ifstream readingFile;
    if(!strcmp(shape, "plane")) {
        x = stof(input[2]);
        z = stof(input[3]);
        div = stoi(input[4]);
        vertexes = plane(x, 0.0, z, div);
    }
    else if(!strcmp(shape, "box")) {
        x = stof(input[2]);
        y = stof(input[3]);
        z = stof(input[4]);
        div = stoi(input[5]);
        vertexes =  box(x, y, z, div);
    }
    else if(!strcmp(shape, "sphere")) {
        radius = stof(input[2]);
        slices = stoi(input[3]);
        stacks = stoi(input[4]);
        vertexes = sphere(radius, slices, stacks);
    }
    else if(!strcmp(shape, "cone")) {
        radius = stof(input[2]);
        height = stof(input[3]);
        slices = stoi(input[4]);
        stacks = stoi(input[5]);
        vertexes = cylinder(radius, 0.0f, height, slices, stacks);
    }
    else if(!strcmp(shape, "cylinder")) {
        r1 = stof(input[2]);
        r2 = stof(input[3]);
        height = stof(input[4]);
        slices = stoi(input[5]);
        stacks = stoi(input[5]);
        vertexes = cylinder(r1, r2, height, slices, stacks);
    }
    else if(!strcmp(shape, "torus")) {
        r1 = stof(input[2]);
        r2 = stof(input[3]);
        sides = stoi(input[4]);
        rings = stoi(input[5]);
        vertexes = torus(r1, r2, sides, rings);
    }
    else if(!strcmp(shape, "ruby")) {
        r1 = stof(input[2]);
        r2 = stof(input[3]);
        h1 = stof(input[4]);
        h2 = stof(input[5]);
        slices = stoi(input[6]);
        stacks = stoi(input[7]);
        vertexes = ruby(r1, r2, h1, h2, slices, stacks);
    }
    else if(!strcmp(shape, "crown")) {
        r1 = stof(input[2]);
        r2 = stof(input[3]);
        sides = stoi(input[4]);
        vertexes = crown(r1, r2, sides);
    }
    else if(!strcmp(shape, "bezier")) {
        file = input[2];
        r1 = stoi(input[3]);
        vertexes = bezier(file, r1);
    }
    return vertexes;
}



int main(int argc, char **argv) {
    vector<string> *vertexes;
    ofstream file;
    string folder = "../shapes/";
    if(!verifyInput(argv) || argc > 9) {
        cout << "Input error\n";
        return -1;
    }
    vertexes = getVertexes(argv);
    string fileName(argv[argc-1]);
    file.open(folder + fileName);
    if(file.is_open()) {
        //print vertexes
        file << vertexes[0].size() << "\n";
        vector<string>::iterator it;
        for(it = vertexes[0].begin(); it != vertexes[0].end(); ++it)
            file << *it;
        //print normals
        file << vertexes[1].size() << "\n";
        for(it = vertexes[1].begin(); it != vertexes[1].end(); ++it)
            file << *it;
        //print textures
    }
    else cout << "Can't write to file\n";
    vertexes[0].clear();
    return 0;
}
