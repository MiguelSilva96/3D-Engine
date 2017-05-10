#include "xmlParser.h"

XmlParser::XmlParser(const char *filename) {
    doc   = new tinyxml2::XMLDocument();
    error = doc -> LoadFile(filename);
    root  = doc -> FirstChildElement("scene");
    elem  = nullptr;
    n = -1;
}


vector<string> XmlParser::split(string str, char delim) {
    string buf;
    stringstream ss(str);
    vector<string> tokens;
    while(ss >> buf)
        tokens.push_back(buf);
    return tokens;
}

vector<Vertex> XmlParser::getPoints(tinyxml2::XMLElement* el) {
    vector<Vertex> points;
    tinyxml2::XMLElement* auxEl;
    float x,y,z;
    auxEl = el;
    auxEl = auxEl -> FirstChildElement();
    for(; auxEl != nullptr; auxEl = auxEl -> NextSiblingElement()) {
        auxEl -> QueryFloatAttribute("X", &x);
        auxEl -> QueryFloatAttribute("Y", &y);
        auxEl -> QueryFloatAttribute("Z", &z);
        points.push_back(Vertex(x,y,z));

    }
    return points;
}

vector<pair<Color*,File*>> XmlParser::getCurVertexes(void) {
    tinyxml2::XMLElement* auxEl;
    vector<string> aux;
    vector<pair<Color*,File*>> vertexes;
    string line;
    string file = "../shapes/";
    pair<Color*, File*> p;

    int nlinhas;
    float x, y, z;
    float r, g, b;
    auxEl = elem;
    auxEl = auxEl -> FirstChildElement("models");
    if(auxEl)
        auxEl = auxEl -> FirstChildElement("model");

    for(; auxEl != nullptr; auxEl = auxEl -> NextSiblingElement()) {
        vector<Vertex> vert;
        vector<Vertex> norm;
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
        nlinhas = stoi(line);

        while(nlinhas > 0) {
            getline(filedisc, line);
            aux = XmlParser::split(line, ' ');
            x = stof(aux.at(0));
            y = stof(aux.at(1));
            z = stof(aux.at(2));
            vert.push_back(Vertex(x,y,z));
            nlinhas--;
        }

        if(getline(filedisc, line))
            nlinhas = stoi(line);

        while(nlinhas > 0) {
            getline(filedisc, line);
            aux = XmlParser::split(line, ' ');
            x = stof(aux.at(0));
            y = stof(aux.at(1));
            z = stof(aux.at(2));
            norm.push_back(Vertex(x,y,z));
            nlinhas--;
        }
        File* f = new File(vert, norm);
        p = make_pair(c, f);
        vertexes.push_back(p);
        loadedFiles[file] = f;
        file = "../shapes/";
    }
    return vertexes;
}

vector<Transformation*> XmlParser::getCurTransformations(void) {
    tinyxml2::XMLElement* auxEl;
    vector<Transformation*> transforms;
    float x, y, z, angle, time = -1;
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
            auxEl -> QueryFloatAttribute("time", &time);
            if(time != -1) {
                vector<Vertex> points;
                points = XmlParser::getPoints(auxEl);
                t = new TranslateCR(x, y, z, time, points);
            }
            else
                t = new Translate(x, y, z);
            transforms.push_back(t);
            time = -1;
        }

        else if(!strcmp(value, "rotate")) {
            auxEl -> QueryFloatAttribute("axisX", &x);
            auxEl -> QueryFloatAttribute("axisY", &y);
            auxEl -> QueryFloatAttribute("axisZ", &z);
            auxEl -> QueryFloatAttribute("angle", &angle);
            auxEl -> QueryFloatAttribute("time", &time);
            if(time != -1)
                t = new RotateAnim(x, y, z, angle, time);
            else
                t = new Rotate(x, y, z, angle);
            transforms.push_back(t);
            time = -1;
        }

        else if(!strcmp(value, "scale")) {
            auxEl -> QueryFloatAttribute("X", &x);
            auxEl -> QueryFloatAttribute("Y", &y);
            auxEl -> QueryFloatAttribute("Z", &z);
            t = new Scale(x, y, z);
            transforms.push_back(t);    
        }

        else if(!strcmp(value, "random")) {
            auxEl -> QueryFloatAttribute("N", &n);
            auxEl -> QueryFloatAttribute("inR", &inR);
            auxEl -> QueryFloatAttribute("outR", &outR);
            auxEl -> QueryFloatAttribute("minScale", &minScale);
            auxEl -> QueryFloatAttribute("maxScale", &maxScale);
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
    Group res  = Group(vertexes, transforms, subgroups);
    if(n > 0) {
        res.n = n;
        res.outR = outR;
        res.inR = inR;
        res.minScale = minScale;
        res.maxScale = maxScale;
        n = -1;
    }
    return res;
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
