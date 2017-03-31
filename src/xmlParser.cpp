#include "xmlParser.h"

XmlParser::XmlParser(const char *filename) {
    doc   = new tinyxml2::XMLDocument();
    error = doc -> LoadFile(filename);
    root  = doc -> FirstChildElement("scene");
    elem  = nullptr;
}


vector<string> XmlParser::split(string str, char delim) {
    string buf;
    stringstream ss(str);
    vector<string> tokens;
    while(ss >> buf)
        tokens.push_back(buf);
    return tokens;
}

vector<pair<Color*,File*>> XmlParser::getCurVertexes(void) {
    tinyxml2::XMLElement* auxEl;
    vector<string> aux;
    vector<pair<Color*,File*>> vertexes;
    string line;
    string file = "../shapes/";
    pair<Color*, File*> p;

    float x, y, z;
    float r, g, b;
    auxEl = elem;
    auxEl = auxEl -> FirstChildElement("models");
    auxEl = auxEl -> FirstChildElement("model");

    for(; auxEl != nullptr; auxEl = auxEl -> NextSiblingElement()) {
        vector<Vertex> vert;
        ifstream filedisc;

        auxEl -> QueryFloatAttribute("diffR", &r);
        auxEl -> QueryFloatAttribute("diffG", &g);
        auxEl -> QueryFloatAttribute("diffB", &b);
        if(!auxEl -> Attribute("diffR"))
            r = g = b = 1.0f;
        Color* c =  new Color(r, g, b);

        file.append(auxEl -> Attribute("file"));
        if(loadedFiles.find(file) != loadedFiles.end()) {
            p = make_pair(c, loadedFiles[file]);
            vertexes.push_back(p);
            continue;
        }

        filedisc.open(file);
        getline(filedisc, line);
    
        while(getline(filedisc, line)) {
            aux = XmlParser::split(line, ' ');
            x = stof(aux.at(0));
            y = stof(aux.at(1));
            z = stof(aux.at(2));
            vert.push_back(Vertex(x,y,z));
        }
        File* f = new File(vert);
        p = make_pair(c, f);
        vertexes.push_back(p);
        loadedFiles[file] = f;
    }
    return vertexes;
}

vector<Transformation*> XmlParser::getCurTransformations(void) {
    tinyxml2::XMLElement* auxEl;
    vector<Transformation*> transforms;
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
    vector<pair<Color*,File*>> vertexes;
    vector<Transformation*> transforms;
    vector<Group> subgroups;
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