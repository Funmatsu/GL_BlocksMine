#pragma once
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "DirectionalLight.h"
#include "PointLIght.h"

using namespace std;
class glShader
{
public:
	int pointLightCount;
	glShader();
	~glShader();
	void createShaderFromString(const char* vertexCode, const char* fragmentCode);
	void createShaderFromFiles(const char* vertexFilePath, const char* fragmentFilePath);
	void createShaderFromFiles(const char* computeFilePath);
	unsigned int getProjectionLocation();
	unsigned int getModelLocation();
	unsigned int getViewLocation();
	unsigned int getAmbientIntensityLocation();
	unsigned int getAmbientColorLocation();
	unsigned int getDiffuseIntensityLocation();
	unsigned int getDirectionLocation();
	unsigned int getColorMaskLocation();
	unsigned int getOrthoLocation();
	unsigned int getUiposLocation();

	struct {
		int uniformColor;
		int uniformAmbientIntensity, uniformDiffuseIntensity;
		int uniformDirection;
	} uniformDirectionalLight;

	int uniformPointLightCount;

	struct {
		int uniformColor;
		int uniformAmbientIntensity, uniformDiffuseIntensity;
		int uniformPosition;

		int uniformConstant, uniformLinear, uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLight, unsigned int lightCount);
	void setTexture(GLenum texture_unit);
	void setDirectionalShadowMap(GLenum texture_unit);
	void setDirectionalLightTransform(mat4 lTransform);

	unsigned int getShaderId() { return shaderId; }

	string readShaderFiles(const char* fileLocation);

	void useShader();
	void clearShader();

private:
	unsigned int shaderId, uniformModel, uniformProjection, uniformView, 
		         uniformDirectionalLightTransform, uniformDirectionalShadowMap,
				 uniformTexture, uniformColorMask, uniformOrtho, uniformUipos;
	unsigned int compileShader(unsigned int type, const char* source);
	void addShader(const char* vertexCode, const char* fragmentCode);
	void addShader(const char* computeCode);
};

class glComputeShader {
public:
	uint id;
	int size;
	uint ssbo[2];
	uint ubo[1];
	vector<vec4> center_radius;
	vector<uint> visibility;
	vec4 planes[6];

	glComputeShader() {
		glGenBuffers(2, ssbo);
		glGenBuffers(1, ubo);
	}
	~glComputeShader();
	void createShaderFromFiles(const char* computeFilePath);
	unsigned int compileShader(unsigned int type, const char* source);
	void addShader(const char* computeCode);

	unsigned int getShaderId() { return id; }
	string readShaderFiles(const char* fileLocation);

	void useShader();
	void clearShader();

	void creatBuffers(int num) {
		//ssbo.resize(num);

		size = num;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[0]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[1]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint) * size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);

		glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * 6, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo[0]);
	}

	void useBuffer(int index) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[index]);
		glDispatchCompute(size, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void useBuffers() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo[0]);

		glDispatchCompute(size, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	vector<uint> readBuffer(int index = 1) {
		vector<uint> vis(size, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[1]);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint) * size, vis.data());
		return vis;
	}

	void writeBuffer(vector<vec4>& cen_rad) {
		size = cen_rad.size();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[0]);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(vec4) * size, cen_rad.data());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
	}

	void writeBuffer(vec4* planes) {
		glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec4) * 6, planes);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo[0]);
	}

	void runBuffer() {
		glDispatchCompute((size + 1023)/1024, 1, 1);
		GL_MAX_COMPUTE_WORK_GROUP_SIZE;
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
};