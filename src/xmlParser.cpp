#include "xmlParser.h"
#include <iostream>

XmlParser::XmlParser(const char *filename) {
    doc   = new tinyxml2::XMLDocument();
    error = doc -> LoadFile(filename);
    root  = doc -> FirstChildElement("scene");
    elem  = nullptr;
}


std::vector<std::string> XmlParser::split(std::string str, char delim) {
    std::string buf;
    std::stringstream ss(str);
    std::vector<std::string> tokens;
    while(ss >> buf)
        tokens.push_back(buf);
    return tokens;
}

std::vector<Vertex> XmlParser::getCurVertexes(void) {
    tinyxml2::XMLElement* auxEl;
    std::vector<std::string> aux;
    std::vector<Vertex> vertexes;
    std::string line;
    std::string file = "../shapes/";
    float x, y, z;
    auxEl = elem;
    auxEl = auxEl -> FirstChildElement("models");
    auxEl = auxEl -> FirstChildElement("model");

    for(; auxEl != nullptr; auxEl = auxEl -> NextSiblingElement()) {
        file.append(auxEl -> Attribute("file"));
        std::ifstream filedisc;
        filedisc.open(file);
        getline(filedisc, line);
    
        while(getline(filedisc, line)) {
            aux = XmlParser::split(line, ' ');
            x = stof(aux.at(0));
            y = stof(aux.at(1));
            z = stof(aux.at(2));
            vertexes.push_back(Vertex(x,y,z));
        }
    }
    return vertexes;
}

std::vector<Transformation*> XmlParser::getCurTransformations(void) {
    tinyxml2::XMLElement* auxEl;
    std::vector<Transformation*> transforms;
    float x, y, z, angle;
    auxEl = elem;
    auxEl = auxEl -> FirstChildElement();
    if(!auxEl)
        return transforms;
    
    while(auxEl) {
        Transformation* t;
        const char* value = auxEl -> Name();

        if(!strcmp(value, "translate")) {
            auxEl -> QueryFloatAttribute("X", &x);
            auxEl -> QueryFloatAttribute("Y", &y);
            auxEl -> QueryFloatAttribute("Z", &z);
            t = new Translate(x, y, z);
            transforms.push_back(t);
        }

        if(!strcmp(value, "rotate")) {
            auxEl -> QueryFloatAttribute("X", &x);
            auxEl -> QueryFloatAttribute("Y", &y);
            auxEl -> QueryFloatAttribute("Z", &z);
            auxEl -> QueryFloatAttribute("A", &angle);
            t = new Rotate(x, y, z, angle);
            transforms.push_back(t);
        }

        if(!strcmp(value, "scale")) {
            auxEl -> QueryFloatAttribute("X", &x);
            auxEl -> QueryFloatAttribute("Y", &y);
            auxEl -> QueryFloatAttribute("Z", &z);
            t = new Scale(x, y, z);
            transforms.push_back(t);    
        }

        auxEl = auxEl -> NextSiblingElement();
    }
    return transforms;
}

Group XmlParser::getGroup(void) {
    tinyxml2::XMLElement* auxEl;
    std::vector<Vertex> vertexes;
    std::vector<Transformation*> transforms;
    std::vector<Group> subgroups;
    auxEl = elem;

    transforms = XmlParser::getCurTransformations();
    vertexes   = XmlParser::getCurVertexes();
    elem = elem -> FirstChildElement("group");
    
    while(elem) {
        subgroups.push_back(XmlParser::getGroup());
        elem = elem -> NextSiblingElement();
    }

    elem = auxEl;
    return Group(vertexes, transforms, subgroups);
}


bool XmlParser::startNextGroup(void) {
    if(!elem)
        elem = root -> FirstChildElement("group");
    else    
        elem = elem -> NextSiblingElement();

    if(elem)
        return true;
    return false;
}

bool XmlParser::readError(void) {
    return error != 0;
}