#pragma once
#include "Light.h"

class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);
    void useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation, GLfloat directionLocation);

    mat4 calcLightTransform();
    vec3 getShadowPos() { return shadowPos; }
    void setShadowPos(vec3 shadPos);
    ~DirectionalLight();
private:
    vec3 direction;
    vec3 shadowPos;
};

