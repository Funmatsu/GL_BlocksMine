#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "Texture.h"

void GL_Texture::setupIcon(unsigned char** pixels, int& width, int& height) {
	*pixels = stbi_load("../textures/block_atlas_31.png", &width, &height, nullptr, 4);
}

void GL_Texture::freeIcon(unsigned char* pixels) {
	//stbi_image_free(pixels);
}

GL_Texture::GL_Texture() {
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = ""; 
}

GL_Texture::GL_Texture(const char* fileLocationString) {
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLocationString;
}
void GL_Texture::loadTexture(){
	stbi_set_flip_vertically_on_load(1);

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 4);
	if (!texData) {
		
		cout << "Failed to find \"" << fileLocation << "\"" << endl;
		return;
	}
	else {
		cout << "Found \"" << fileLocation << "\"" << endl;
	}
	pixels.assign(texData, texData + width * height * 4);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);
}
void GL_Texture::useTexture(){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
bool GL_Texture::pixelOpaque(int x, int y) {
	return pixels[4 * (y * width + x) + 3] > 250;
}
void GL_Texture::useNextTexture() {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void GL_Texture::unbindNextTexture() {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GL_Texture::useTexture(GLenum tex) {
	glActiveTexture(tex);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void GL_Texture::clearTexture(){
	glDeleteTextures(1, &textureId);
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

GL_Texture::~GL_Texture() {
	clearTexture();
}