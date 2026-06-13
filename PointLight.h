#pragma once
#include "Light.h"

const int MAX_POINT_LIGHTS = 100;
extern unsigned int pointLightCount;

class PointLight :
    public Light
{
public:
    PointLight();
    ~PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
               float xPos, float yPos, float zPos,
               float con, float lin, float exp);
    void useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation,    
                  float positionLocation,
                  float conLoc, float linLoc, float expLoc);
private:
    vec3 position;
    float constant, linear, exponent;
};