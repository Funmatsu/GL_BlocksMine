#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cmath>
#include <vector>

using namespace std;

struct MeshAttrContainer {
	unsigned int vbo, ibo, vao;
	unsigned int indexCount;
	MeshAttrContainer() {
		vao = 0, vbo = 0, ibo = 0;
		indexCount = 0;
		//glGenVertexArrays(1, &vao);
	}
	MeshAttrContainer(const MeshAttrContainer& mesh) {
		vao = mesh.vao, vbo = mesh.vbo, ibo = mesh.ibo;
		indexCount = mesh.indexCount;
	}
	void operator=(MeshAttrContainer mesh) {
		vao = mesh.vao, vbo = mesh.vbo, ibo = mesh.ibo;
		indexCount = mesh.indexCount;
	}
	bool operator==(MeshAttrContainer mesh) {
		return (vao == mesh.vao && vbo == mesh.vbo && ibo == mesh.ibo);
	}
	void clearMesh() {
		if (ibo != 0) {
			glDeleteBuffers(1, &ibo);
			ibo = 0;
		}
		if (vbo != 0) {
			glDeleteBuffers(1, &vbo);
			vbo = 0;
		}
		if (vao != 0) {
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}
		indexCount = 0;
	}
	~MeshAttrContainer() {
		clearMesh();
	}
};

class LightMesh
{
public:
	LightMesh();
	void createMesh(vector<float>& vertices, vector<unsigned int>& indices, unsigned int verticesCount, unsigned int indicesCount);
	void createMesh(vector<float>& vertices, vector<unsigned int>& indices);
	void createMesh(float* vertsdata, unsigned int* indsdata, size_t indsize, size_t vertsize);
	void renderMesh();
	void renderMeshAsLines();
	void clearMesh();
	void giveMesh();
	//void incRefcount();
	//void decRefcount();
	//~LightMesh();
	LightMesh(const LightMesh& mesh) {
		meshAttrs = mesh.meshAttrs;
	}
	void operator=(LightMesh mesh) {
		meshAttrs = mesh.meshAttrs;
	}
	bool operator==(LightMesh mesh) {
		return meshAttrs == mesh.meshAttrs;
	}
	bool operator!=(LightMesh mesh) {
		return !(meshAttrs == mesh.meshAttrs);
	}
	//static void initVAOs();
	//~LightMesh() {
	//	meshAttrs.reset();
	//}
private:
	shared_ptr<MeshAttrContainer> meshAttrs;
	//unsigned int vao, vbo, ibo, indexCount;
	//static unsigned int refcnt;
};

