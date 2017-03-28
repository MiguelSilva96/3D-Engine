#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vertex.h"
//#include "transformation.h"
#include "tinyxml2.h"


class XmlParser {
    private:
        char *filename;
        tinyxml2::XMLDocument* doc;
        tinyxml2::XMLElement* elem;
        tinyxml2::XMLNode* root;
        tinyxml2::XMLError error;

        std::vector<std::string> split(std::string str, char delim);

    public:
        XmlParser(const char *filename);
        //bool startNextShape(void);
        //bool hasChildGroup(void);
        //Transformation getNextTransformation(void);
        std::vector<Vertex> getCurShapeVertexes(void);
        bool readError(void);
};

#endif