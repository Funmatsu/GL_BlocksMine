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

//vec2 oords(uint coord) {
//	return vec2(int16_t((coord >> 16)), int16_t(coord));
//}
//uint tooords(ivec2 chunkCoord) {
//	return ((uint16_t(chunkCoord.x)) << 16) | (uint16_t(chunkCoord.y));
//}
//
//int main() {
//	cout << to_string(oords(4294049806));// << " " << to_string(oords((tooords(vec2(-3, -3)))));
//}
int main(){
	startMenu startMenu;
	startMenu.launch();
	Game game;
	game.run();
}