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


class XmlParser {
    private:
        tinyxml2::XMLDocument* doc;
        tinyxml2::XMLElement* elem;
        tinyxml2::XMLNode* root;
        tinyxml2::XMLError error;
        std::map<std::string, File*> loadedFiles;

        std::vector<std::string> split(std::string str, char delim);
        std::vector<Transformation*> getCurTransformations(void);
        std::vector<File*> getCurVertexes(void);

    public:
        XmlParser(XmlParser*);
        XmlParser(const char*);
        bool startNextGroup(void);
        Group getGroup(void);
        bool readError(void);
};

#endif