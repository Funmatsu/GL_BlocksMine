#include "DirectionalLight.h"
float off = 200.0f;

DirectionalLight::DirectionalLight() : Light() { lightProj = ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f); }
DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity) {
	direction = vec3(xDir, yDir, zDir);
	
	lightProj = ortho(-off - shadowPos.x, off - shadowPos.x, -off + shadowPos.z, off + shadowPos.z, 0.01f, 2500.0f);
}
void DirectionalLight::useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation, GLfloat directionLocation) {
	glUniform3f(aColorLocation, color.x, color.y, color.z);
	glUniform1f(aIntensityLocation, ambientIntensity);
	glUniform1f(dIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

mat4 DirectionalLight::calcLightTransform() {
	vec3 forward = normalize(direction);
	mat4 lightView = lookAt(-forward*200.f, /*front*/forward, /*up*/vec3(0,1,0));
	//vec2 texelSize = vec2((2.0f * (off)) / 1920, (2.0f * (off)) / 1059);
	//vec4 cameraLight = lightView * vec4(shadowPos, 1);
	//cameraLight.x = floor(cameraLight.x / texelSize.x) * texelSize.x;
	//cameraLight.y = floor(cameraLight.y / texelSize.y) * texelSize.y;
	//
	//mat4 view = lookAt(vec3(inverse(lightView) * cameraLight), vec3(inverse(lightView) * cameraLight) + direction, vec3(0, 1, 0));
	directionalLightTransform = lightProj * lightView;
	return directionalLightTransform;
}

void DirectionalLight::setShadowPos(vec3 shadowpos) {
	shadowPos = shadowpos;
	lightProj = ortho(-off - shadowPos.x, off - shadowPos.x, -off + shadowPos.z, off + shadowPos.z, 0.01f, 500.0f);
}

DirectionalLight::~DirectionalLight() {}

