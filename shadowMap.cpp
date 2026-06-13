#include "shadowMap.h"

shadowMap::shadowMap() { fbo = 0; shadow_map = 0; }

bool shadowMap::init(int width, int height) {
	shadowWidth = width, shadowHeight = height; 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glViewport(0, 0, width, height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glGenTextures(1, &shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadow_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (stat != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Framebuffer not complete! " << stat << " error here" << endl;
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, shadowWidth, shadowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}
void shadowMap::write(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void shadowMap::read(GLenum texture_unit){
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
}

shadowMap::~shadowMap(){
	cout << "destructor called!" << endl;
	if(fbo)
		glDeleteFramebuffers(1, &fbo);
	if (shadow_map)
		glDeleteTextures(1, &shadow_map);
}