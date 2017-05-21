#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tinyxml2.h"
#include "group.h"
#include "lights.h"

using namespace std;

class XmlParser {
    private:
        tinyxml2::XMLDocument* doc;
        tinyxml2::XMLElement* elem;
        tinyxml2::XMLNode* root;
        tinyxml2::XMLError error;
        map<string, File*> loadedFiles;
        float n, outR, inR, minScale, maxScale;

        vector<string> split(string str, char delim);
        vector<Transformation*> getCurTransformations(void);
        vector<pair<Color**,File*>> getCurVertexes(void);
        vector<Vertex> getPoints(tinyxml2::XMLElement*);
        Color** getColor(tinyxml2::XMLElement*);
        unsigned int getTexture(tinyxml2::XMLElement*);
		char* getLabel(tinyxml2::XMLElement*);

    public:
        XmlParser(XmlParser*);
        XmlParser(const char*);
        bool startNextGroup(void);
        Group getGroup(void);
        bool readError(void);
        vector<Light*> getLights(void); 
};

#endif