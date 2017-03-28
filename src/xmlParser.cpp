#include "xmlParser.h"


XmlParser::XmlParser(const char *filename) {
    doc = new tinyxml2::XMLDocument();
    error = doc -> LoadFile(filename);
    root = doc -> FirstChildElement("scene");
}


std::vector<std::string> XmlParser::split(std::string str, char delim) {
    std::string buf;
    std::stringstream ss(str);
    std::vector<std::string> tokens;
    while(ss >> buf)
        tokens.push_back(buf);
    return tokens;
}

std::vector<Vertex> XmlParser::getCurShapeVertexes(void) {
	std::vector<std::string> aux;
	std::vector<Vertex> vertexes;
	float x, y, z;
	elem = root -> FirstChildElement("model");
    for(; elem != NULL; elem = elem -> NextSiblingElement()) {
        std::string line;
        std::string file = "../shapes/";
        
        file.append(elem -> Attribute("file"));
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

bool XmlParser::readError(void) {
	return error != 0;
}