#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include "shapes.h"


/**
    Auxiliar function that returns a string that
    describes one vertex with the coordinates given
    as arguments.

    @param x Position on X axis
    @param y Position on Y axis
    @param z Position on Z axis
    @return String that describes the vertex
*/

string vertexString(float x, float y, float z) {
    stringstream strStream;
    strStream << x << " ";
    strStream << y << " ";
    strStream << z << "\n";
    return strStream.str();
}

string vertexString(float x, float y) {
    stringstream strStream;
    strStream << x << " ";
    strStream << y << "\n";
    return strStream.str();
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

float getPoint(float u, float v, float m[4][4], float p[4][4]) {
    float pValue = 0;
    float aux[4], aux2[4];

    //bu*M
    for(int i = 0; i<4; i++){
        aux[i]  = (powf(u,3.0)*m[0][i]);
        aux[i] += (powf(u,2.0)*m[1][i]);
        aux[i] += (u*m[2][i]) + m[3][i];
    }

    //(bu*M)*P
    for(int i = 0; i<4; i++){
        aux2[i]  = (aux[0]*p[0][i]);
        aux2[i] += (aux[1]*p[1][i]);
        aux2[i] += (aux[2]*p[2][i]);
        aux2[i] += (aux[3]*p[3][i]);
    }

    //((bu*M)*P)*MT
    for(int i = 0; i<4; i++){
        aux[i]  = (aux2[0]*m[0][i]);
        aux[i] += (aux2[1]*m[1][i]);
        aux[i] += (aux2[2]*m[2][i]);
        aux[i] += (aux2[3]*m[3][i]);
    }

    //(((bu*M)*P)*MT)*bv
    pValue  = aux[0] * powf(v, 3.0);
    pValue += aux[1] * powf(v, 2.0);
    pValue += aux[2] * v;
    pValue += aux[3];

    return pValue;
}

float getUTangent(float u, float v, float m[4][4], float p[4][4]) {
    float tValue = 0;
    float aux[4], aux2[4];
    //u*M
    for(int i = 0; i<4; i++){
        aux[i]  = 3*(powf(u,2.0)*m[0][i]);
        aux[i] += 2*u*m[1][i];
        aux[i] += m[2][i];
    }
    //(u*M)*P
    for(int i = 0; i<4; i++){
        aux2[i]  = (aux[0]*p[0][i]);
        aux2[i] += (aux[1]*p[1][i]);
        aux2[i] += (aux[2]*p[2][i]);
        aux2[i] += (aux[3]*p[3][i]);
    }
    //((u*M)*P)*MT
    for(int i = 0; i<4; i++){
        aux[i]  = (aux2[0]*m[0][i]);
        aux[i] += (aux2[1]*m[1][i]);
        aux[i] += (aux2[2]*m[2][i]);
        aux[i] += (aux2[3]*m[3][i]);
    }
    //(((bu*M)*P)*MT)*VT
    tValue  = aux[0] * powf(v, 3.0);
    tValue += aux[1] * powf(v, 2.0);
    tValue += aux[2] * v;
    tValue += aux[3];
    return tValue;
}

float getVTangent(float u, float v, float m[4][4], float p[4][4]) {
    float tValue = 0;
    float aux[4], aux2[4];
    //u*M
    for(int i = 0; i<4; i++){
        aux[i]  = (powf(u,3.0)*m[0][i]);
        aux[i] += (powf(u,2.0)*m[1][i]);
        aux[i] += (u*m[2][i]) + m[3][i];
    }
    //(u*M)*P
    for(int i = 0; i<4; i++){
        aux2[i]  = (aux[0]*p[0][i]);
        aux2[i] += (aux[1]*p[1][i]);
        aux2[i] += (aux[2]*p[2][i]);
        aux2[i] += (aux[3]*p[3][i]);
    }
    //((u*M)*P)*MT
    for(int i = 0; i<4; i++){
        aux[i]  = (aux2[0]*m[0][i]);
        aux[i] += (aux2[1]*m[1][i]);
        aux[i] += (aux2[2]*m[2][i]);
        aux[i] += (aux2[3]*m[3][i]);
    }
    //(((bu*M)*P)*MT)*v
    tValue  = aux[0] * (3 * powf(v,2.0));
    tValue += aux[1] * (2 * v);
    tValue += aux[2];
    return tValue;
}

vector<string> bezierNormals(int tessellation, vector<vector<int>> indices, vector<vector<float>> controlPoints) {
    vector<string> vr;
    int aux, nrPatches = indices.size();
    vector<int> patchIndice;
    vector<vector<float>> ma;
    float px[4][4], py[4][4], pz[4][4], res[3], resU[3], resV[3];
    float u, v, level = 1.0f/tessellation;
    for(int patch = 0; patch < nrPatches; patch++) {
        aux = 0;
        patchIndice = indices.at(patch);

        //Obter os pontos de controlo do patch atual
        for(int i = 0; i < 16; i++) {
            ma.push_back(controlPoints.at(patchIndice[i]));
        }

        //Preencher px, py, e pz com valores dos pontos de controlo
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++, aux++){
                px[i][j] = ma.at(aux).at(0);
                py[i][j] = ma.at(aux).at(1);
                pz[i][j] = ma.at(aux).at(2);
            }
        }

        float m[4][4] = { 
                          {-1,  3, -3, 1},
                          { 3, -6,  3, 0},
                          {-3,  3,  0, 0},
                          { 1,  0,  0, 0} 
                        };

        for(u = 0.0f; u < 1 ; u += level){
            for(v = 0.0f; v < 1 ; v += level){
                resU[0] = getUTangent(u, v, m, px);
                resU[1] = getUTangent(u, v, m, py);
                resU[2] = getUTangent(u, v, m, pz);
                resV[0] = getVTangent(u, v, m, px);
                resV[1] = getVTangent(u, v, m, py);
                resV[2] = getVTangent(u, v, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                resU[0] = getUTangent(u+level,v+level, m, px);
                resU[1] = getUTangent(u+level,v+level, m, py);
                resU[2] = getUTangent(u+level,v+level, m, pz);
                resV[0] = getVTangent(u+level,v+level, m, px);
                resV[1] = getVTangent(u+level,v+level, m, py);
                resV[2] = getVTangent(u+level,v+level, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                resU[0] = getUTangent(u+level,v, m, px);
                resU[1] = getUTangent(u+level,v, m, py);
                resU[2] = getUTangent(u+level,v, m, pz);
                resV[0] = getVTangent(u+level,v, m, px);
                resV[1] = getVTangent(u+level,v, m, py);
                resV[2] = getVTangent(u+level,v, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                resU[0] = getUTangent(u, v, m, px);
                resU[1] = getUTangent(u, v, m, py);
                resU[2] = getUTangent(u, v, m, pz);
                resV[0] = getVTangent(u, v, m, px);
                resV[1] = getVTangent(u, v, m, py);
                resV[2] = getVTangent(u, v, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                resU[0] = getUTangent(u,v+level, m, px);
                resU[1] = getUTangent(u,v+level, m, py);
                resU[2] = getUTangent(u,v+level, m, pz);
                resV[0] = getVTangent(u,v+level, m, px);
                resV[1] = getVTangent(u,v+level, m, py);
                resV[2] = getVTangent(u,v+level, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                resU[0] = getUTangent(u+level,v+level, m, px);
                resU[1] = getUTangent(u+level,v+level, m, py);
                resU[2] = getUTangent(u+level,v+level, m, pz);
                resV[0] = getVTangent(u+level,v+level, m, px);
                resV[1] = getVTangent(u+level,v+level, m, py);
                resV[2] = getVTangent(u+level,v+level, m, pz);
                normalize(resU);
                normalize(resV);
                cross(resV, resU, res);
                vr.push_back(vertexString(res[0],res[1],res[2]));
            }
        }
        ma.clear();
    }
    return vr;
}

vector<string> bezierSurface(int tessellation, vector<vector<int>> indices, vector<vector<float>> controlPoints) {
    vector<string> vr;
    int aux, nrPatches = indices.size();
    vector<int> patchIndice;
    vector<vector<float>> ma;
    float px[4][4], py[4][4], pz[4][4], res[3];
    float u, v, level = 1.0f/tessellation;
    for(int patch = 0; patch < nrPatches; patch++) {
        aux = 0;
        patchIndice = indices.at(patch);

        //Obter os pontos de controlo do patch atual
        for(int i = 0; i < 16; i++) {
            ma.push_back(controlPoints.at(patchIndice[i]));
        }

        //Preencher px, py, e pz com valores dos pontos de controlo
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++, aux++){
                px[i][j] = ma.at(aux).at(0);
                py[i][j] = ma.at(aux).at(1);
                pz[i][j] = ma.at(aux).at(2);
            }
        }

        float m[4][4] = { 
                          {-1,  3, -3, 1},
                          { 3, -6,  3, 0},
                          {-3,  3,  0, 0},
                          { 1,  0,  0, 0} 
                        };

        for(u = 0.0f; u < 1 ; u += level){
            for(v = 0.0f; v < 1 ; v += level){
                res[0] = getPoint(u, v, m, px);
                res[1] = getPoint(u, v, m, py);
                res[2] = getPoint(u, v, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                res[0] = getPoint(u+level,v+level, m, px);
                res[1] = getPoint(u+level,v+level, m, py);
                res[2] = getPoint(u+level,v+level, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                res[0] = getPoint(u+level,v, m, px);
                res[1] = getPoint(u+level,v, m, py);
                res[2] = getPoint(u+level,v, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                res[0] = getPoint(u, v, m, px);
                res[1] = getPoint(u, v, m, py);
                res[2] = getPoint(u, v, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                res[0] = getPoint(u,v+level, m, px);
                res[1] = getPoint(u,v+level, m, py);
                res[2] = getPoint(u,v+level, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));

                res[0] = getPoint(u+level,v+level, m, px);
                res[1] = getPoint(u+level,v+level, m, py);
                res[2] = getPoint(u+level,v+level, m, pz);
                vr.push_back(vertexString(res[0],res[1],res[2]));
            }
        }
        ma.clear();
    }
    return vr;
}


vector<int> addIndex(string line) {
    vector<int> colIndices;
    string token;
    size_t pos = 0;
    string delimiter = ", ";
    while((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        int indexValue = stoi(token);
        colIndices.push_back(indexValue);
    }
    int indexValue = stoi(line);
    colIndices.push_back(indexValue);
    return colIndices;
}

vector<float> addPoint(string line) {
    vector<float> colPoints;
    string token;
    size_t pos = 0;
    string delimiter = ", ";
    while((pos = line.find(delimiter)) != string::npos) {
      token = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
      float pointValue = stof(token);
      colPoints.push_back(pointValue);
    }
    float pointValue = stof(line);
    colPoints.push_back(pointValue);
    return colPoints;
}

vector<string>* bezier(char *input, int tessellation) {
    vector<string> *v = new vector<string>[3];
    ifstream readingFile(input);
    int nrPatches = 0;
    int nrControlPoints = 0;
    vector<int> colIndices;
    vector<float> colPoints;
    vector<vector<int>> indices;
    vector<vector<float>> controlPoints;
    string line;
    int i = 0;
    while(getline(readingFile, line)) {
        if (i == 0) {
            nrPatches = stoi(line);
        }
        else if (i > 0 && i <= nrPatches) {
            colIndices =  addIndex(line);
            indices.push_back(colIndices);
        }
        else if (i == nrPatches + 1) {
            nrControlPoints = stoi(line);
        }
        else {
            colPoints = addPoint(line);
            controlPoints.push_back(colPoints);
        }
        i++;
    }
    v[0] = bezierSurface(tessellation, indices, controlPoints);
    v[1] = bezierNormals(tessellation, indices, controlPoints);
    return v;
}


vector<string>* plane(float x, float y, float z, int div) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    for(float i = 0; i < div; i++) {
        for(float j = 0; j < div; j++) {
            v[0].push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v[0].push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
            v[0].push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*j));

            v[0].push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v[0].push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*(j+1)));
            v[0].push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
        }
    }
    return v;
}


vector<string>* box(float x, float y, float z, int div) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    //top
    v = plane(x, y/2, z, div);
    for(float i = 0; i < div; i++) {
        for(float j = 0; j < div; j++) {
            //base
            v[0].push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v[0].push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*j));
            v[0].push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));

            v[0].push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v[0].push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));
            v[0].push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*(j+1)));

            v[1].push_back(vertexString(0,-1,0));
            v[1].push_back(vertexString(0,-1,0));
            v[1].push_back(vertexString(0,-1,0));
            v[1].push_back(vertexString(0,-1,0));
            v[1].push_back(vertexString(0,-1,0));
            v[1].push_back(vertexString(0,-1,0));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            //front
            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, z/2));

            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));

            v[1].push_back(vertexString(1,0,0));
            v[1].push_back(vertexString(1,0,0));
            v[1].push_back(vertexString(1,0,0));
            v[1].push_back(vertexString(1,0,0));
            v[1].push_back(vertexString(1,0,0));
            v[1].push_back(vertexString(1,0,0));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            //back
            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, -z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));

            v[0].push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));
            v[0].push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), -z/2));

            v[1].push_back(vertexString(-1,0,0));
            v[1].push_back(vertexString(-1,0,0));
            v[1].push_back(vertexString(-1,0,0));
            v[1].push_back(vertexString(-1,0,0));
            v[1].push_back(vertexString(-1,0,0));
            v[1].push_back(vertexString(-1,0,0));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            //right
            v[0].push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v[0].push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v[0].push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*i));

            v[0].push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v[0].push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
            v[0].push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));

            v[1].push_back(vertexString(0,0,1));
            v[1].push_back(vertexString(0,0,1));
            v[1].push_back(vertexString(0,0,1));
            v[1].push_back(vertexString(0,0,1));
            v[1].push_back(vertexString(0,0,1));
            v[1].push_back(vertexString(0,0,1));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            //left
            v[0].push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v[0].push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*i));
            v[0].push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));

            v[0].push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v[0].push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v[0].push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));

            v[1].push_back(vertexString(0,0,-1));
            v[1].push_back(vertexString(0,0,-1));
            v[1].push_back(vertexString(0,0,-1));
            v[1].push_back(vertexString(0,0,-1));
            v[1].push_back(vertexString(0,0,-1));
            v[1].push_back(vertexString(0,0,-1));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,j/div));
            v[2].push_back(vertexString((i+1)/div,j/div));
            v[2].push_back(vertexString((i+1)/div,(j+1)/div));
            v[2].push_back(vertexString(i/div,(j+1)/div));
        }
    }
    return v;
}


vector<string>* sphere(float radius, int slices, int stacks) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    double teta, fi;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            if (i != stacks - 1) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v[0].push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta)*sin(fi)));
                v[2].push_back(vertexString((float)j/slices,1 - (float)i/stacks));

                fi = (i+1) * M_PI/stacks;
                teta = (j+1) * 2 * M_PI/slices;
                v[0].push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta)*sin(fi)));
                v[2].push_back(vertexString((float)(j+1)/slices,1 - (float)(i+1)/stacks));
                teta = j * 2 * M_PI/slices;
                v[0].push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta)*sin(fi)));
                v[2].push_back(vertexString((float)j/slices,1 - (float)(i+1)/stacks));
            }
            if (i != 0) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v[0].push_back(vertexString(radius*cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta)*sin(fi)));
                v[2].push_back(vertexString((float)j/slices,1 - (float)i/stacks));
                teta = (j + 1) * 2 * M_PI/slices;
                v[0].push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta) * sin(fi)));
                v[2].push_back(vertexString((float)(j+1)/slices,1 - (float)i/stacks));
                fi = (i + 1) * M_PI/stacks;
                v[0].push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                v[1].push_back(vertexString(cos(teta)*sin(fi),
                            cos(fi),
                            sin(teta) * sin(fi)));
                v[2].push_back(vertexString((float)(j+1)/slices, 1 - (float)(i+1)/stacks));
            }
        }
    }
    return v;
}


vector<string>* cylinder(float radB, float radT, float height, int slices, int stacks) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    float alpha = (float)2* M_PI / slices;
    float h = (float)height / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas
    //draw base
    for (int j = 1; j <= slices; j++) {
        v[0].push_back(vertexString(sin(alpha*(j-1))*radB,
                    0.0f, cos(alpha*(j-1))*radB));
        v[1].push_back(vertexString(sin(alpha*(j - 1)),1.0f, cos(alpha*(j - 1))));
		v[2].push_back(vertexString((float)(sin(alpha*(j-1)) + 1.0f)*0.5, (float)(cos(alpha*(j-1)) + 1.0f)*0.5));
        angle = alpha * j;
        v[0].push_back(vertexString(0.0f, 0.0f, 0.0f));
        v[1].push_back(vertexString(0.0f,1.0f,0.0f));
		v[2].push_back(vertexString(0.5f, 0.5f));
        v[0].push_back(vertexString(sin(angle)*radB, 0.0f, cos(angle)*radB));
        v[1].push_back(vertexString(sin(angle),1.0f, cos(angle)));
		v[2].push_back(vertexString((float)(sin(angle) + 1.0f)*0.5, (float)(cos(angle) + 1.0f)*0.5));
        prev_lvl = radB;
        //draw sides
        for (int i = 1; i <= stacks; i++){
			lvl = radB - (float)(radB - radT)*i / stacks;
			v[0].push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
				h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
            v[1].push_back(vertexString(sin(alpha*(j - 1)), 0, cos(alpha*(j - 1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float) (i-1)/stacks));
			v[0].push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
            v[2].push_back(vertexString((float)j/slices,(float) (i-1)/stacks));
			v[0].push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
            v[1].push_back(vertexString(sin(alpha*(j - 1)), 0, cos(alpha*(j - 1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float) i/stacks));
			v[0].push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
            v[1].push_back(vertexString(sin(alpha*(j - 1)), 0, cos(alpha*(j - 1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float) i/stacks));
			v[0].push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
			v[2].push_back(vertexString((float)j/slices,(float) (i-1)/stacks));
            v[0].push_back(vertexString(lvl * sin(angle),
				h*i, lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
			prev_lvl = lvl;
            v[2].push_back(vertexString((float)j/slices,(float) i/stacks));
        }
    }
    //draw top
    if (radT) {
        for (int j = 1; j <= slices; j++) {
            angle = alpha * j;
            v[0].push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
            v[1].push_back(vertexString(sin(angle),1.0f,cos(angle)));
			v[2].push_back(vertexString((float)(sin(angle) + 1.0f)*0.5, (float)(cos(angle) + 1.0f)*0.5));
            angle = alpha * (j - 1);
            v[0].push_back(vertexString(0.0f, height, 0.0f));
            v[1].push_back(vertexString(0.0f,1.0f,0.0f));
            v[2].push_back(vertexString(0.5f, 0.5f));
            v[0].push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
            v[1].push_back(vertexString(sin(angle),1.0f,cos(angle)));
            v[2].push_back(vertexString((float)(sin(angle)+1.0f)*0.5,(float)(cos(angle)+1.0f)*0.5));
        }
    }
    return v;
}


vector<string>* torus(float inner, float outer, int sides, int rings) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    float alpha = (float) 2 * M_PI / rings;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    for (int i = 1; i <= rings; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)(i-1)/rings,(float) (j-1)/sides));
            angle2 = beta * j;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)(i-1)/rings,(float) j/sides));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)i/rings,(float) (j-1)/sides));
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)i/rings,(float) (j-1)/sides));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)(i-1)/rings,(float) j/sides));
            angle1 = alpha * i;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                                        r * sin(angle2),
                                        cos(angle1)*(outer - r*cos(angle2))));
            v[1].push_back(vertexString(sin(angle1)*(1-cos(angle2)),
                                        sin(angle2),
                                        cos(angle1)*(1-cos(angle2))));
            v[2].push_back(vertexString((float)i/rings,(float) j/sides));
        }
    }
    return v;
}

vector<string>* ruby(float rb, float rt, float heightb, float heightt, int slices, int stacks) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    int i, j;
    float alpha = (float)2 * M_PI / slices;
    float h = (float)heightb / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas

    v = cylinder(rb, rt, heightt, slices, stacks);
    for (j = 1; j <= slices; j++) {
        angle = alpha * j;
        prev_lvl = rb;
        //draw sides
        for (i = 1; i <= stacks; i++) {
            lvl = rb - (float)rb*i / stacks;
			v[0].push_back(vertexString(prev_lvl * sin(angle),
				-h*(i - 1), prev_lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
            v[2].push_back(vertexString((float)j/slices,(float) (i-1)/stacks));
            v[0].push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
                        -h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
            v[1].push_back(vertexString(sin(alpha*(j-1)), 0, cos(alpha*(j-1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float)(i-1)/stacks));
            v[0].push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
            v[1].push_back(vertexString(sin(alpha * (j - 1)), 0, cos(alpha * (j - 1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float) i/stacks));
            v[0].push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
            v[1].push_back(vertexString(sin(alpha * (j - 1)), 0, cos(alpha * (j - 1))));
            v[2].push_back(vertexString((float)(j-1)/slices,(float) i/stacks));
			v[0].push_back(vertexString(lvl * sin(angle),
				-h * i, lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
            v[2].push_back(vertexString((float)j/slices,(float)i/stacks));
			v[0].push_back(vertexString(prev_lvl * sin(angle),
                        -h * (i - 1), prev_lvl * cos(angle)));
            v[1].push_back(vertexString(sin(angle), 0, cos(angle)));
            prev_lvl = lvl;
            v[2].push_back(vertexString((float)j/slices,(float)(i-1)/stacks));
        }
    }
    return v;
}


vector<string>* crown(float inner, float outer, int sides) {
    // 0 - vertices; 1 - normals; 2 - texCoords
    vector<string> *v = new vector<string>[3];
    float alpha = (float) 2 * M_PI / sides;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    for (float i = 1; i <= sides; i++) {
        for (float j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i-1)/sides,(j-1)/sides));
            angle2 = beta * j;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i-1)/sides,(j)/sides));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i)/sides,(j-1)/sides));
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i)/sides,(j-1)/sides));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i-1)/sides,(j)/sides));
            angle1 = alpha * i;
            v[0].push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v[2].push_back(vertexString((i)/sides,(j)/sides));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
            v[1].push_back(vertexString(0,1,0));
        }
    }
    return v;
}
