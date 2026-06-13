#pragma once
#include "shader.h"

static const char* vshader           = "shaders/vshader.glsl";
static const char* fshader           = "shaders/fshader.glsl";

static const char* vshader2          = "shaders/vshader2.glsl";
static const char* fshader2          = "shaders/fshader2.glsl";

static const char* vshaderInv        = "shaders/vshaderInv.txt";
static const char* fshaderInv        = "shaders/fshaderInv.txt";

static const char* vshaderInvBlock   = "shaders/vshaderInvBlock.txt";
static const char* fshaderInvBlock   = "shaders/fshaderInvBlock.txt";

static const char* vshaderInvSlot    = "shaders/vshaderInvSlot.glsl";
static const char* fshaderInvSlot    = "shaders/fshaderInvSlot.glsl";

static const char* vshaderLooking    = "shaders/vshaderLooking.txt";
static const char* fshaderLooking    = "shaders/fshaderLooking.txt";

static const char* shadowMapvshader  = "shaders/shadowMapvshader.txt";
static const char* shadowMapfshader  = "shaders/shadowMapfshader.txt";

static const char* vshaderProjectile = "shaders/vshaderProjectile.txt";
static const char* fshaderProjectile = "shaders/fshaderProjectile.txt";

static const char* vshaderCompass    = "shaders/vshaderCompass.txt";
static const char* fshaderCompass    = "shaders/fshaderCompass.txt";

static const char* vshaderCloud    = "shaders/vshaderCloud.txt";
static const char* fshaderCloud    = "shaders/fshaderCloud.txt";

static const char* cshader = "shaders/cshader.glsl";

inline vector<glShader*> shaders;
inline glShader* directionalShadowShader;
inline glShader* projectileShader;
inline glShader* compassShader;
inline glShader* cloudShader;
inline glComputeShader* computeShader;

inline void createShaders() {
    //computeShader = new glComputeShader();
	//computeShader->createShaderFromFiles(cshader);

    glShader* shader1 = new glShader();
    shader1->createShaderFromFiles(vshader, fshader);
    shaders.push_back(shader1);

    glShader* shader2 = new glShader();
    shader2->createShaderFromFiles(vshader2, fshader2);
    shaders.push_back(shader2);

    glShader* shaderInv = new glShader();
    shaderInv->createShaderFromFiles(vshaderInv, fshaderInv);
    shaders.push_back(shaderInv);

    glShader* shaderInvBlock = new glShader();
    shaderInvBlock->createShaderFromFiles(vshaderInvBlock, fshaderInvBlock);
    shaders.push_back(shaderInvBlock);

    glShader* shaderInvSlot = new glShader();
    shaderInvSlot->createShaderFromFiles(vshaderInvSlot, fshaderInvSlot);
    shaders.push_back(shaderInvSlot);

    glShader* shaderLooking = new glShader();
    shaderLooking->createShaderFromFiles(vshaderLooking, fshaderLooking);
    shaders.push_back(shaderLooking);

    glShader* shaderCloud = new glShader();
    shaderCloud->createShaderFromFiles(vshaderCloud, fshaderCloud);
    shaders.push_back(shaderCloud);

    directionalShadowShader = new glShader();
    directionalShadowShader->createShaderFromFiles(shadowMapvshader, shadowMapfshader);

    projectileShader = new glShader();
    projectileShader->createShaderFromFiles(vshaderProjectile, fshaderProjectile);

    compassShader = new glShader();
    compassShader->createShaderFromFiles(vshaderCompass, fshaderCompass);

    cloudShader = new glShader();
    cloudShader->createShaderFromFiles(vshaderCloud, fshaderCloud);
}