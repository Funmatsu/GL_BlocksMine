#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc/type_ptr.hpp>
#include <vector>

using namespace std;

#define BLOCK_TEX           0
#define MAIN_INV_TEX        1
#define SLOT_TEX            2
#define LARGE_INV_TEX       3
#define TOOLS_TEX           4
#define CRAFT_GUI_TEX       5
#define TOP_TEX             6
#define FACE_TEX            7
#define BREAK_STAGE_TEX     8
#define TEXT_TEX            9
#define FOLL_TEX            10
#define UI_BUTTON_TEX       11

class GL_Texture;

inline vector<unique_ptr<GL_Texture>> Textures;

class GL_Texture
{
public:
	GL_Texture();
	GL_Texture(const char* fileLocation);
	static void setupIcon(unsigned char** pixels, int& width, int& height);
	static void freeIcon(unsigned char* pixels);
	void loadTexture();
	void useTexture();
	bool pixelOpaque(int x, int y);
	void useTexture(GLenum tex);
	void useNextTexture();
	void unbindNextTexture();
	void clearTexture();
	int getWidth() { return width; }
	int getHeight() { return height; }
	~GL_Texture();
	vector<unsigned char> pixels;
private:
	const char* fileLocation;
	unsigned int textureId;
	int width, height, bitDepth;	
};

inline void addTextures() {
	Textures.push_back(make_unique<GL_Texture>("textures/block_atlas_32.png"));//#define BLOCK_TEX            0
	Textures.push_back(make_unique<GL_Texture>("textures/clear_toolbar_2.png"));//#define MAIN_INV_TEX        1
	Textures.push_back(make_unique<GL_Texture>("textures/clear_toolbar_3.png"));//#define SLOT_TEX            2
	Textures.push_back(make_unique<GL_Texture>("textures/main_inventory.png"));//#define LARGE_INV_TEX        3
	Textures.push_back(make_unique<GL_Texture>("textures/tools_atlas_4.png"));//#define TOOLS_TEX             4
	Textures.push_back(make_unique<GL_Texture>("textures/crafting_table_gui.png"));//#define CRAFT_GUI_TEX    5
	Textures.push_back(make_unique<GL_Texture>("textures/block_overlay_2.png"));//#define TOP_TEX             6
	Textures.push_back(make_unique<GL_Texture>("textures/steve_face.jpg"));//#define FACE_TEX                 7
	Textures.push_back(make_unique<GL_Texture>("textures/break_stage_3.png"));//#define BLOCK_STAGES_TEX      8
	Textures.push_back(make_unique<GL_Texture>("textures/text.png"));//#define TEXT_TEX					      9
	Textures.push_back(make_unique<GL_Texture>("textures/foliage.png"));//#define FOLL_TEX			          10
	Textures.push_back(make_unique<GL_Texture>("textures/stone_UI_button.png"));//#define FOLL_TEX            11
	//Textures.push_back(new Texturegl("textures\\inventory_base.png"));

	for (int i = BLOCK_TEX; i < Textures.size(); i++) { Textures[i]->loadTexture(); }
}