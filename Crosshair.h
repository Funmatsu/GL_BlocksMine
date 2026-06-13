#pragma once
#include <vector>
#include "LightMesh.h"

extern int WIDTH ;
extern int HEIGHT;

extern float centerX;
extern float centerY;
extern float crosshairSize;

class Crosshair
{
public:
    void defineCrosshairGeometry();
    void drawCrosshair();

private:
    vector<glm::vec2> crosshairVertices;
    vector<GLfloat> vers;
    vector<unsigned int> inds;
    LightMesh crosshair;
    GLuint crosshairVbo = 0;
    GLuint crosshairVao = 0;
    GLuint crosshairIbo = 0;
};

