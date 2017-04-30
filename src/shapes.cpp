#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
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



std::string vertexString(float x, float y, float z) {
    std::stringstream strStream;
    strStream << x << " ";
    strStream << y << " ";
    strStream << z << "\n";
    return strStream.str();
}

std::vector<std::vector<float>> preenche(int i, std::vector<std::vector<int>> indices,
                                    std::vector<std::vector<float>> controlPoints) {

    int t = 0;
    std::vector<int> linha = indices[i];
    std::vector<std::vector<float>> setDeControlo;
    for(int j = 0; j <= 3; j++) {
      for(int k = 0; k <= 3; k++) {
        int n = linha[t];
        setDeControlo.push_back(controlPoints[n]);
        t++;
      }
    }
    return setDeControlo;
}

void constroiTeapot(std::vector<std::vector<int>> indices,
                    std::vector<std::vector<float>> controlPoints,
                    int nrPatches) {

    std::vector<int> vertices;
    std::vector<std::vector<float>> pontosDeControlo;
    for(int i = 1; i < 2; i++) { // não é para ficar assim
       pontosDeControlo = preenche(i, indices, controlPoints);
    }
    for (int i = 0; i < pontosDeControlo.size(); i++) {
        for (int j = 0; j < pontosDeControlo[i].size(); j++) {
          printf("%f ", pontosDeControlo[i][j]);
        }
    printf("\n");
    }
}


std::vector<int> addIndex(std::string line) {
    std::vector<int> colIndices;
    std::string token;
    size_t pos = 0;
    std::string delimiter = ", ";
    while((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
      int indexValue = std::stoi(token);
      colIndices.push_back(indexValue);
    }
    int indexValue = std::stoi(line);
    colIndices.push_back(indexValue);
    return colIndices;
}

std::vector<float> addPoint(std::string line) {
    std::vector<float> colPoints;
    std::string token;
    size_t pos = 0;
    std::string delimiter = ", ";
    while((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
      float pointValue = std::stof(token);
      colPoints.push_back(pointValue);
    }
    float pointValue = std::stof(line);
    colPoints.push_back(pointValue);
    return colPoints;
}

std::vector<std::string> teatopOrComet(char *input) {
  std::vector<std::string> v;
  std::ifstream readingFile(input);
  int nrPatches = 0;
  int nrControlPoints = 0;
  int i = 0;
  std::vector<int> colIndices;
  std::vector<float> colPoints;
  std::vector<std::vector<int>> indices;
  std::vector<std::vector<float>> controlPoints;
  std::string line;
  while(getline(readingFile, line)) {
    if (i == 0) {
      nrPatches = std::stoi(line);
    }
    else if (i > 0 && i <= nrPatches) {
      colIndices =  addIndex(line);
      indices.push_back(colIndices);
    }
    else if (i == nrPatches + 1) {
      nrControlPoints = std::stoi(line);
    }
    else {
      colPoints = addPoint(line);
      controlPoints.push_back(colPoints);
    }
    i++;
  }
/* Só para teste
  printf("%d\n", nrPatches);
  for (int i = 0; i < indices.size(); i++) {
      for (int j = 0; j < indices[i].size(); j++) {
        printf("%d ", indices[i][j]);
      }
  printf("\n");
  }
  printf("%d\n", nrControlPoints);
  for (int i = 0; i < controlPoints.size(); i++)
  {
      for (int j = 0; j < controlPoints[i].size(); j++)
      {
          printf("%f ", controlPoints[i][j] );
      }
  printf("\n");
}
*/
  //chamar função que constroi o gajo
  constroiTeapot(indices,controlPoints, nrPatches);
  return v;
}


std::vector<std::string> plane(float x, float y, float z, int div) {
    std::vector<std::string> v;
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*j));

            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
        }
    }
    return v;
}


std::vector<std::string> box(float x, float y, float z, int div) {
    std::vector<std::string> v;
    //top
    v = plane(x, y/2, z, div);
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            //base
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));

            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*(j+1)));
            //front
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            //back
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), -z/2));
            //right
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*i));

            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            //left
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));

            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
        }
    }
    return v;
}


std::vector<std::string> sphere(float radius, int slices, int stacks) {
    std::vector<std::string> v;
    double teta, fi;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            if (i != stacks - 1) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                fi = (i+1) * M_PI/stacks;
                teta = (j+1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
            }
            if (i != 0) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius*cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = (j + 1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                fi = (i + 1) * M_PI/stacks;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
            }
        }
    }
    return v;
}


std::vector<std::string> cylinder(float radB, float radT, float height, int slices, int stacks) {
    std::vector<std::string> v;
    float alpha = (float)2* M_PI / slices;
    float h = (float)height / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas
    //draw base
    for (int j = 1; j <= slices; j++) {
        v.push_back(vertexString(sin(alpha*(j-1))*radB,
                    0.0f, cos(alpha*(j-1))*radB));
        angle = alpha * j;
        v.push_back(vertexString(0.0f, 0.0f, 0.0f));
        v.push_back(vertexString(sin(angle)*radB, 0.0f, cos(angle)*radB));
        prev_lvl = radB;
        //draw sides
        for (int i = 1; i <= stacks; i++){
			lvl = radB - (float)(radB - radT)*i / stacks;
			v.push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
				h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
			v.push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
			v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
			v.push_back(vertexString(lvl * sin(angle),
				h*i, lvl * cos(angle)));
			prev_lvl = lvl;
        }
    }
    //draw top
    if (radT) {
        for (int j = 1; j <= slices; j++) {
            angle = alpha * j;
            v.push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
            angle = alpha * (j - 1);
            v.push_back(vertexString(0.0f, height, 0.0f));
            v.push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
        }
    }
    return v;
}


std::vector<std::string> torus(float inner, float outer, int sides, int rings) {
    float alpha = (float) 2 * M_PI / rings;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    std::vector<std::string> v;
    for (int i = 1; i <= rings; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
        }
    }
    return v;
}

std::vector<std::string> ruby(float rb, float rt, float heightb, float heightt, int slices, int stacks) {
    int i, j;
    float alpha = (float)2 * M_PI / slices;
    float h = (float)heightb / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas
    std::vector<std::string> v;
    v = cylinder(rb, rt, heightt, slices, stacks);
    for (j = 1; j <= slices; j++) {
        angle = alpha * j;
        prev_lvl = rb;
        //draw sides
        for (i = 1; i <= stacks; i++) {
            lvl = rb - (float)rb*i / stacks;
			v.push_back(vertexString(prev_lvl * sin(angle),
				-h*(i - 1), prev_lvl * cos(angle)));
            v.push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
                        -h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
            v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
            v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(lvl * sin(angle),
				-h * i, lvl * cos(angle)));
			v.push_back(vertexString(prev_lvl * sin(angle),
                        -h * (i - 1), prev_lvl * cos(angle)));
            prev_lvl = lvl;
        }
    }
    return v;
}


std::vector<std::string> crown(float inner, float outer, int sides) {
    float alpha = (float) 2 * M_PI / sides;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    std::vector<std::string> v;
    for (int i = 1; i <= sides; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
        }
    }
    return v;
}
