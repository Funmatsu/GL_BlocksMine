#pragma once
#include "libraries.h"
#include "Cloud.h"

inline std::unordered_set<uint32_t> cloudCoords;

static const char* vshaderSky = "C:\\Users\\Honla\\Desktop\\OpenGL_Udemy\\shaders/vshaderSky.txt";
static const char* fshaderSky = "C:\\Users\\Honla\\Desktop\\OpenGL_Udemy\\shaders/fshaderSky.txt";

static const char* vshaderother = "C:\\Users\\Honla\\Desktop\\OpenGL_Udemy\\shaders/vshaderSky.txt";
static const char* fshaderother = "C:\\Users\\Honla\\Desktop\\OpenGL_Udemy\\shaders/fshaderSky.txt";

class Sky
{
public:
	vector<pair<uint32, LightMesh>> clouds;
	vector<float> skyVerts = {
		-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
																
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
																
		-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
																
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
																
		-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
																
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,
	};

public:
	vector<float> sunVerts = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,	 1.0f,1.0f,1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f,	 1.0f,1.0f,1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f,	 1.0f,1.0f,1.0f,
	};

	vector<unsigned int> skyInds = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
	};

	vector<unsigned int> sunInds = {
			0, 1, 2,
			2, 3, 0,
	};

	glShader shader = glShader();
	glShader sunshader = glShader();

	Sky() {}
	LightMesh skyMesh;
	LightMesh sunMesh;
	void useShader() {
		shader.useShader();
	}
	void buildSky() { 
		shader.createShaderFromFiles(vshaderSky, fshaderSky); 
		sunshader.createShaderFromFiles(vshaderother, fshaderother);
		skyMesh.createMesh(skyVerts, skyInds, skyVerts.size(), skyInds.size());
		sunMesh.createMesh(sunVerts, sunInds, sunVerts.size(), sunInds.size());
	}
	void displaySky() { 
		skyMesh.renderMesh(); 
	}
	void applySky(mat4 view, mat4 projection) {
		glDepthMask(GL_FALSE);        // disable depth writes but could test against depth buffer. remember tat!
		shader.useShader();
		glUniformMatrix4fv(shader.getViewLocation(), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(shader.getProjectionLocation(), 1, GL_FALSE, value_ptr(projection));
		displaySky();
		//sunshader.useShader();
		glUniformMatrix4fv(sunshader.getViewLocation(), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(sunshader.getProjectionLocation(), 1, GL_FALSE, value_ptr(projection));
		//sunMesh.renderMesh();
		glDepthMask(GL_TRUE);
	}
	void addCloud(unique_ptr<CloudMesh>& chm, vec2 coord) {
		//if (!(chm && chm->mesh)) return;
		Mesh& m = *chm->mesh;
		LightMesh lm;
		lm.createMesh(m.vertices.data(), m.indices.data(), m.vertices.size(), m.indices.size());
		clouds.push_back({ pack(coord), lm });
	}
	void renderClouds(vec3 position, int renderDistance) {
		for (auto clIt = clouds.begin(); clIt != clouds.end(); ) {
			ivec2 coords = unpack((*clIt).first);
			LightMesh& chm = clIt->second;
			if ((coords.x >= position.x / chunkSize - renderDistance && coords.x <= position.x / chunkSize + renderDistance) &&
				(coords.y >= position.z / chunkSize - renderDistance && coords.y <= position.z / chunkSize + renderDistance)) {
		
				chm.renderMesh();
			}
			else {
				cloudCoords.erase(pack(coords));
				clIt = clouds.erase(clIt);
				continue;
			}
			clIt++;
		}
	}
};

inline Sky sky;