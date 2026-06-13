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

class Mesh
{
public:
	Mesh(){}
	//void createMesh(vector<float>& vertices, vector<unsigned int>& indices, unsigned int verticesCount, unsigned int indicesCount);
	//void createMesh(vector<float>& vertices, vector<unsigned int>& indices);
	//void createMeshLocally();
	//void renderMesh();
	//void renderMeshAsLines();
	//void clearMesh();
	vector<float> vertices;
	vector<unsigned int> indices;
	//~Mesh();
	Mesh(const Mesh& mesh) {
		//vao = mesh.vao, vbo = mesh.vbo, ibo = mesh.ibo;
		//indexCount = mesh.indexCount;
		vertices = mesh.vertices;
		indices = mesh.indices;
	}
	void operator=(Mesh mesh) {
		//vao = mesh.vao, vbo = mesh.vbo, ibo = mesh.ibo;
		//indexCount = mesh.indexCount;
		vertices = mesh.vertices;
		indices = mesh.indices;
	}
private:
	//unsigned int vao, vbo, ibo;
	//uint32_t indexCount : 24;
};

