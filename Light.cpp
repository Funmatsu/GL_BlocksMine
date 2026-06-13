#include "Light.h"
Light::Light() {
	color = vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}
Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) { 
	shadow_map = new shadowMap();
	shadow_map->init(shadowWidth, shadowHeight);
	color = vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}
void Light::useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation) {
	glUniform3f(aColorLocation, color.x, color.y, color.z);
	glUniform1f(aIntensityLocation, ambientIntensity);
	glUniform1f(dIntensityLocation, diffuseIntensity);
}
Light::~Light(){}