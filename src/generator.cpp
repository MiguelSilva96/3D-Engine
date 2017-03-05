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
    std::regex ints("[-+]?[0-9]+");
    std::regex floats("[-+]?[0-9]+\\.?[0-9]*");
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
    return result;
}

/**
    Takes input to create vertexes as instructed.

    @param input Input strings.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> getVertexes(char **input) {
    char *shape = input[1];
    float x, y, z, radius, height;
    int div, slices, stacks;
    std::vector<std::string> vertexes;
    if(!strcmp(shape, "plane")) {
        x = std::stof(input[2]);
        z = std::stof(input[3]);
        div = std::stoi(input[4]);
        vertexes = plane(x, 0.0, z, div);
    }
    else if(!strcmp(shape, "box")) {
        x = std::stof(input[2]);
        y = std::stof(input[3]);
        z = std::stof(input[4]);
        div = std::stoi(input[5]);
        vertexes =  box(x, y, z, div);
    }
    else if(!strcmp(shape, "sphere")) {
        radius = std::stof(input[2]);
        slices = std::stoi(input[3]);
        stacks = std::stoi(input[4]);
        vertexes = sphere(radius, slices, stacks);
    }
    else if(!strcmp(shape, "cone")) {
        radius = std::stof(input[2]);
        height = std::stof(input[3]);
        slices = std::stoi(input[4]);
        stacks = std::stoi(input[5]);
        vertexes = cylinder(radius, 0.0f, height, slices, stacks);
    }
    return vertexes;
}

int main(int argc, char **argv) {
    std::vector<std::string> vertexes;
    std::ofstream file;
    std::string folder = "../shapes/";
    if(argc < 6 || !verifyInput(argv) || argc > 8) {
        std::cout << "Input error\n";
        return -1;
    }
    vertexes = getVertexes(argv);
    std::string fileName(argv[argc-1]);
    file.open(folder + fileName);
    if(file.is_open()) {
        file << vertexes.size() << "\n";
        std::vector<std::string>::iterator it;
        for(it = vertexes.begin(); it != vertexes.end(); ++it)
            file << *it;
    }
    else std::cout << "Can't write to file";
    vertexes.clear();
    return 0;
}
