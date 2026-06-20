#define GLEW_STATIC

#include "libraries.h"

using namespace std;
using namespace glm;
using json = nlohmann::json;

DirectionalLight mainLight, auxLight;
PointLight pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;

#include "varDef.h"

#include "gameRenderer.h"
#include "startRenderer.h"

int main(){
	startMenu startMenu;
	startMenu.launch();
	Game game;
	game.run();
}