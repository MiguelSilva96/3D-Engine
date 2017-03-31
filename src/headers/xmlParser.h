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

using namespace std;

class XmlParser {
    private:
        tinyxml2::XMLDocument* doc;
        tinyxml2::XMLElement* elem;
        tinyxml2::XMLNode* root;
        tinyxml2::XMLError error;
        map<string, File*> loadedFiles;

        vector<string> split(string str, char delim);
        vector<Transformation*> getCurTransformations(void);
        vector<pair<Color*,File*>> getCurVertexes(void);

    public:
        XmlParser(XmlParser*);
        XmlParser(const char*);
        bool startNextGroup(void);
        Group getGroup(void);
        bool readError(void);
};

#endif