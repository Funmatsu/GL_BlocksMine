#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <span>    

#include <glm/glm.hpp>
#include <glm\gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
using namespace std;
class glBuffer
{
public:
	unsigned int ssbid, ubid;
	vector<int> data;
	const int N = 10;
	glBuffer() {
		ssbid = 0;
		ubid = 0;
	}

	glBuffer(uint32_t ssbloc, uint32_t ubloc) {
		glGenBuffers(1, &ssbid);
		glGenBuffers(1, &ubid);

		int N = 10;
		data.resize(N, 0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbid);
		glBindBuffer(GL_UNIFORM_BUFFER, ubid);

		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * N, data.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(N), &N, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbloc, ssbid);
		glBindBufferBase(GL_UNIFORM_BUFFER, ubloc, ubid);
	}
};