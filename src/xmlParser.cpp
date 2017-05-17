#include "xmlParser.h"

XmlParser::XmlParser(const char *filename) {
    doc   = new tinyxml2::XMLDocument();
    error = doc -> LoadFile(filename);
    root  = doc -> FirstChildElement("scene");
    elem  = nullptr;
    n = -1;
}

Color** XmlParser::getColor(tinyxml2::XMLElement* elem) {
        int col = GL_DIFFUSE;
        unsigned int texID;
        float r, g, b;
        int i = 0;
        Color** colors = new Color*[5];
        r = g = b = 1.0;
        for(int i = 0; i < 5; i++)
            colors[i] = nullptr;
        texID = XmlParser::getTexture(elem);
        if(elem -> Attribute("diffR")) {
            elem -> QueryFloatAttribute("diffR", &r);
            elem -> QueryFloatAttribute("diffG", &g);
            elem -> QueryFloatAttribute("diffB", &b);
            col = GL_DIFFUSE;
            colors[i++] = new Color(r, g, b, col, texID);
        }
        if(elem -> Attribute("specR")) {
            elem -> QueryFloatAttribute("specR", &r);
            elem -> QueryFloatAttribute("specG", &g);
            elem -> QueryFloatAttribute("specB", &b);
            col = GL_SPECULAR;
            colors[i++] = new Color(r, g, b, col, texID);
        }
        if(elem -> Attribute("emisR")) {
            elem -> QueryFloatAttribute("emisR", &r);
            elem -> QueryFloatAttribute("emisG", &g);
            elem -> QueryFloatAttribute("emisB", &b);
            col = GL_EMISSION;
            colors[i++] = new Color(r, g, b, col, texID);
        }
        if(elem -> Attribute("ambiR")) {
            elem -> QueryFloatAttribute("ambiR", &r);
            elem -> QueryFloatAttribute("ambiG", &g);
            elem -> QueryFloatAttribute("ambiB", &b);
            col = GL_AMBIENT;
            colors[i++] = new Color(r, g, b, col, texID);
        }
        if(!i)
            colors[i] = new Color(r, g, b, col, texID);
        return colors;
}

unsigned int XmlParser::getTexture(tinyxml2::XMLElement* elem) {
    unsigned int t,tw,th;
    unsigned char *texData;
    unsigned int texID = 0;
    string s;

    if(elem -> Attribute("texture")) {
        s = elem -> Attribute("texture");

        ilInit();
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        ilGenImages(1,&t);
        ilBindImage(t);
        ilLoadImage((ILstring)s.c_str());
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glGenTextures(1, &texID);
        
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(
            GL_TEXTURE_2D, 
            GL_TEXTURE_WRAP_S, 
            GL_REPEAT
        );
        glTexParameteri(GL_TEXTURE_2D, 
                        GL_TEXTURE_WRAP_T, 
                        GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, 
                        GL_TEXTURE_MAG_FILTER, 
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, 
                        GL_TEXTURE_MIN_FILTER, 
                        GL_LINEAR_MIPMAP_LINEAR);
            
        glTexImage2D(GL_TEXTURE_2D, 0, 
                    GL_RGBA, tw, th, 0, GL_RGBA, 
                    GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

    }
    return texID;
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

vector<pair<Color**,File*>> XmlParser::getCurVertexes(void) {
    tinyxml2::XMLElement* auxEl;
    vector<string> aux;
    vector<pair<Color**,File*>> vertexes;
    string line;
    string file = "../shapes/";
    pair<Color**, File*> p;

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
        vector<float>  txtr;
        ifstream filedisc;

        Color** c = XmlParser::getColor(auxEl);

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

        if(getline(filedisc, line))
            nlinhas = stoi(line);

        while(nlinhas > 0) {
            getline(filedisc, line);
            aux = XmlParser::split(line, ' ');
            x = stof(aux.at(0));
            y = stof(aux.at(1));
            txtr.push_back(x);
            txtr.push_back(y);
            nlinhas--;
        }

        File* f = new File(vert, norm, txtr);
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
    vector<pair<Color**,File*>> vertexes;
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


vector<Light*> XmlParser::getLights(void) {
    tinyxml2::XMLElement* aux;
    vector<Light*> lights;
    float x, y, z;
    Light *light;
    aux = root -> FirstChildElement("lights");
    if(aux)
        aux = aux -> FirstChildElement("light");
    for(; aux != nullptr; aux = aux -> NextSiblingElement()) {
        aux -> QueryFloatAttribute("posX", &x);
        aux -> QueryFloatAttribute("posY", &y);
        aux -> QueryFloatAttribute("posZ", &z);
        const char *type = aux -> Attribute("type");
        if(!strcmp(type, "POINT"))
            light =  new LightPoint(x,y,z);
        else if(!strcmp(type, "DIR"))
            light =  new DirectionalLight(x,y,z);
        lights.push_back(light);
    }
    return lights;
}
