#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm\gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace std;

class shadowMap
{
public:
	shadowMap();

	virtual bool init(int width, int height);
	virtual void write();
	virtual void read(GLenum texture_unit);
	int getShadowWidth() { return shadowWidth; }
	int getShadowHeight() { return shadowHeight; }

	~shadowMap();
	unsigned int fbo;
protected:
	unsigned int shadow_map;
	int shadowWidth = 0, shadowHeight = 0;
};

