#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include "shadowMap.h"

using namespace glm;

class Light {
	public:
		Light();
		Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
		void useLight(GLfloat aIntensityLocation, GLfloat aColorLocation, GLfloat dIntensityLocation);
		shadowMap* getShadowMap() { return shadow_map; }
		shadowMap* shadow_map;
		~Light();
		mat4 directionalLightTransform;

	protected:
		vec3 color;
		GLfloat ambientIntensity;
		GLfloat diffuseIntensity;

		mat4 lightProj;
		
};

