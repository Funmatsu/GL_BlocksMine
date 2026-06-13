#include "PointLight.h"

PointLight::PointLight(){
    constant = 0, linear = 0, exponent = 0;
    position = vec3(0.0f, 1.0f, 0.0f);
}
PointLight::~PointLight(){}
PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
    float xPos, float yPos, float zPos,
    float con, float lin, float exp) : Light(1920, 1059, red, green, blue, aIntensity, dIntensity) {
    position = vec3(xPos, yPos, zPos);
    constant = con, linear = lin, exponent = exp;
}
void PointLight::useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation,
    float positionLocation,
    float conLoc, float linLoc, float expLoc) {
    glUniform3f(aColorLocation, color.x, color.y, color.z);
    glUniform1f(aIntensityLocation, ambientIntensity);
    glUniform1f(dIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(conLoc, constant);
    glUniform1f(linLoc, linear);
    glUniform1f(expLoc, exponent);
}