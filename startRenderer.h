#pragma once
#include "libraries.h"
#include <iostream>
#include "Window.h"
#include "shaderlist.h"
//#include "items.h"
#include "Text.h"

using namespace std;

//unsigned int general_vao = 0;
//unsigned int item_vao = 0;
//bool vaobind = 1;

class startMenu
{
public:
	startMenu() {
		std::ios_base::sync_with_stdio(false); // Disables synch between C and C++ standard streams for performance boost
		mainWindow = Window(WIDTH, HEIGHT);
		mainWindow.initialize();
		createShaders();
		addTextures();

		//glGenVertexArrays(1, &general_vao);
		//glGenVertexArrays(1, &item_vao);

		//glBindVertexArray(general_vao);
		//LightMesh::initVAOs();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	};
	void launch() {
		cout << "Start menu launched" << endl;
		
		shaders[4]->useShader();

		Text welcomeText("Welcome to GLBlocksMines!", vec3(1), vec2(750, 850));
		//UIButton startButton("Start", [](int) { cout << "Pressed Down"; return 0; }, 0);

		mat4 ortho = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT)),
			 model = translate(mat4(1), vec3(650, 725, 0));

		glUniformMatrix4fv(shaders[4]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
		
		glUniform1i(glGetUniformLocation(shaders[4]->getShaderId(), "theTexture"), 0);
		//vector<float> vs = {
		//	/*pos :*/0  , 0  , 0, /*uvs :*/0, 0, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
		//	/*pos :*/0  , 200, 0, /*uvs :*/0, 1, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
		//	/*pos :*/200, 200, 0, /*uvs :*/1, 1, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
		//	/*pos :*/200, 0  , 0, /*uvs :*/1, 0, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1
		//};

		vector<float> vs = {
			0,  0,  0,   /*uvs :*/0,   0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			0,  40, 0,   /*uvs :*/0,   0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 40, 0,   /*uvs :*/0.2, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 0,  0,   /*uvs :*/0.2, 0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			0,  200, 0,  /*uvs :*/0,   0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			0,  240, 0,  /*uvs :*/0,   1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 240, 0,  /*uvs :*/0.2, 1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 200, 0,  /*uvs :*/0.2, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			500, 0,  0,  /*uvs :*/0.8, 0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 40, 0,  /*uvs :*/0.8, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 40, 0,  /*uvs :*/1,   0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 0,  0,  /*uvs :*/1,   0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			500, 200, 0, /*uvs :*/0.8, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 240, 0, /*uvs :*/0.8, 1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 240, 0, /*uvs :*/1,   1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 200, 0, /*uvs :*/1,   0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,



			40,  0,  0,   /*uvs :*/0.2, 0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40,  40, 0,   /*uvs :*/0.2, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 40, 0,   /*uvs :*/0.8, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 0,  0,   /*uvs :*/0.8, 0,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			40,  200, 0,  /*uvs :*/0.2, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40,  240, 0,  /*uvs :*/0.2, 1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 240, 0,  /*uvs :*/0.8, 1,   /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 200, 0,  /*uvs :*/0.8, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			0,  40,  0,  /*uvs :*/0,   0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			0,  200, 0,  /*uvs :*/0,   0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 200, 0,  /*uvs :*/0.2, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			40, 40,  0,  /*uvs :*/0.2, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			500, 40,  0, /*uvs :*/0.8, 0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			500, 200, 0, /*uvs :*/0.8, 0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 200, 0, /*uvs :*/1,   0.8, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,
			540, 40,  0, /*uvs :*/1,   0.2, /*transparency? :*/1, /*normals? :*/1, 1, 0, /*color : cyan*/1, 1, 1,

			40,  40,  0, /*uvs :*/0.2, 0.2, /*transparency? :*/0, /*normals? :*/0.2, 0.8, 0, /*color : cyan*/1, 1, 1,
			40,  200, 0, /*uvs :*/0.2, 0.8, /*transparency? :*/0, /*normals? :*/0.2, 0.8, 0, /*color : cyan*/1, 1, 1,
			500, 200, 0, /*uvs :*/4.8, 0.8, /*transparency? :*/0, /*normals? :*/0.2, 0.8, 0, /*color : cyan*/1, 1, 1,
			500, 40,  0, /*uvs :*/4.8, 0.2, /*transparency? :*/0, /*normals? :*/0.2, 0.8, 0, /*color : cyan*/1, 1, 1,
		};

		vector<unsigned int> is = {0, 1, 2, 
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
								   22, 23, 20,
		
								   24, 25, 26,
								   26, 27, 24,
		
								   28, 29, 30,
								   30, 31, 28, 
		
								   32, 33, 34,
								   34, 35, 32};

		LightMesh m;
		m.createMesh(vs, is, vs.size(), is.size());
		while(!mainWindow.getKeys()[GLFW_KEY_X]) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0, 0, 1, 0);
			glfwPollEvents();
			glUniformMatrix4fv(shaders[4]->getModelLocation(), 1, GL_FALSE, glm::value_ptr(model * scale(mat4(1), vec3(1, 1, 1))));
			Textures[UI_BUTTON_TEX]->useTexture();
			m.renderMesh();
			welcomeText.drawonly();
			//TODO : Implement start menu
			//cout << mainWindow.getXPos() << " x " << mainWindow.getYPos() << " y " << endl;
			mainWindow.swapBuffers();	
		}
	}
private:
};

