#version 430 core
                                       
out vec4 color;
in vec4 pdc;
in vec2 texCoords;
flat in float transparency;
in vec3 normal;
in vec3 FragPos;
in vec3 colorMask;
in vec4 directionalLightSpacePos;  
int bufferWidth = 1920	;
int bufferHeight = 1059;

const int MAX_POINT_LIGHTS = 100;
struct Light{
		vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight{
	Light base;
    vec3 direction;
};

struct PointLight{
	Light base;
    vec3 position;
	float constant, linear, exponent;
};

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform sampler2D topTexture;
uniform sampler2D breakStageTexture;
uniform sampler2D grassColorTexture;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int pointLightCount;

uniform vec3 camPos;
uniform float fogStart;
uniform float fogEnd;

uniform int  uniformBreaking;
uniform vec3 uniformBreakPos;

//vec2 poissonDisk[16] = vec2[](
//    vec2( -0.94201624, -0.39906216 ),
//    vec2(  0.94558609, -0.76890725 ),
//    vec2( -0.09418410, -0.92938870 ),
//    vec2(  0.34495938,  0.29387760 ),
//    vec2( -0.91588581,  0.45771432 ),
//    vec2( -0.81544232, -0.87912464 ),
//    vec2( -0.38277543,  0.27676845 ),
//    vec2(  0.97484398,  0.75648379 ),
//    vec2(  0.44323325, -0.97511554 ),
//    vec2(  0.53742981, -0.47373420 ),
//    vec2( -0.26496911, -0.41893023 ),
//    vec2(  0.79197514,  0.19090188 ),
//    vec2( -0.24188840,  0.99706507 ),
//    vec2( -0.81409955,  0.91437590 ),
//    vec2(  0.19984126,  0.78641367 ),
//    vec2(  0.14383161, -0.14100790 )
//);
//
//float findBlockerDepth(float receiverDepth) {
//    float blockerSum = 0.0;
//    int blockerCount = 0;
//	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
//	projCoords = (projCoords * 0.5) + 0.5;
//	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
//    for (int i = 0; i < 8; i++) {
//        float sampleDepth = texture(directionalShadowMap, projCoords.xy + poissonDisk[i] * texelSize).r;
//        if (sampleDepth < receiverDepth) {
//            blockerSum += sampleDepth;
//            blockerCount++;
//        }
//    }
//
//    return blockerCount > 0 ? blockerSum / blockerCount : -1.0;
//}
//
//float computePenumbra(float receiverDepth, float blockerDepth) {
//    float pen = (receiverDepth - blockerDepth) / blockerDepth;
//    pen = max(pen, 0.0);
//    return clamp(pen * 100.0, 0.0, 300.0); // tweak 30 / 40 to taste
//}
//
//float pcfShadow(float penumbra) {
//	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
//	projCoords = (projCoords * 0.5) + 0.5;
//	float current = projCoords.z,
//	      shadow = 0.0, bias = 0.0001;
//    int samples = 8;
//	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
//
//    for (int i = 0; i < samples; i++) {
//        vec2 offset = poissonDisk[i] * penumbra * texelSize;
//        float sampleDepth = texture(directionalShadowMap, projCoords.xy + offset).r;
//        shadow += sampleDepth < projCoords.z - bias ? 1.1 : 0.25;
//    }
//
//    return shadow / samples;
//}
//
//float calcDirectionalShadowFactor() {
//    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
//    projCoords = projCoords * 0.5 + 0.5;
//
//    float receiverDepth = projCoords.z;
//
//    float blockerDepth = findBlockerDepth(receiverDepth);
//    if (blockerDepth < 0.0) { return 0.0; }
//
//    float penumbra = computePenumbra(receiverDepth, blockerDepth);
//
//    return pcfShadow(penumbra);
//}

float calcDirectionalShadowFactor(){
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;
	float current = projCoords.z,
//		  closest = texture(directionalShadowMap, projCoords.xy).r,
	      shadow = 0.0, bias = 0.0001;
//	shadow = current - bias > closest ? 1.0f : 0.0;
	//vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	vec2 texelSize = vec2(1.0 / (1920*1.0), 1 / (1059*1.0));
	int var = 3;
	for(int x = -var; x <= var; ++x) {
		for(int y = -var; y <= var; ++y) {
			float pcfDepth = texture(directionalShadowMap,
									 projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1 : 0;
		}
	}
	shadow /= pow(2*var + 1, 2); // average
	//shadow = smoothstep(0.0, 1.0, shadow);

	return shadow;
}

vec4 calcLightByDirection(Light light, vec3 direction, float shadowFactor){
	vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0f);
        float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
	return vec4((ambientColor * (1 - shadowFactor*0.4) + (1 - shadowFactor) * diffuseColor).xyz, 1.0f);
}
vec4 calcDirLight(){ 
	float shadowFactor = calcDirectionalShadowFactor();
	//float shadowFactor = 1.0f;
	return calcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}
vec4 calcPointLight() {
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++){
		vec3 direction = -(pointLights[i].position - FragPos);
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = calcLightByDirection(pointLights[i].base, direction, 0.0);
		float attenuation = (pointLights[i].exponent * (distance * distance) +
							 pointLights[i].linear * distance +
							 pointLights[i].constant);
		totalColor += 2*(color) / attenuation;
	}
	
	return totalColor;
}

float LinearizeDepth(float depth, float near, float far) {
    return (2.0 * near) / (far + near - depth * (far - near));
}

void main(){
	uint uvs = floatBitsToUint(transparency);
	uint tileSize = (uvs >> 16) & 0xFFu;

	vec2 uv = vec2((uvs & 0xFFu), (uvs >> 8) & 0xFFu);
	vec2 uvCoords = (fract(texCoords) + uv) / float(tileSize);
	vec4 tex = texture(theTexture, uvCoords);
	if (tex.a < 0.6) discard;
	
	float lightFactor = 1.0f;
	if(normal.y == -1) lightFactor *= 1.0f;
	else if(normal.y == 1) lightFactor *= 0.20f;
	if(normal.z != 0) lightFactor *= 0.75f;
	else lightFactor *= 0.90f; //if(normal.x != 0)
	
	uint light = (uvs >> 24u) & 0xFFu;
	
	vec4 finalColor = calcDirLight()*lightFactor;//*(float(light) * lightFactor)/64.f;// * clamp((float(light) * lightFactor + 100)/255, 0, 1);
	finalColor += calcPointLight();
	float dist = length(vec3(FragPos.x - camPos.x, (FragPos.y - camPos.y)/2 , FragPos.z - camPos.z));
	//float fadeStart = 0.5f * (fogEnd + fogStart);
	//float fadeFactor = clamp((dist - fadeStart)/(fogEnd - fadeStart), 0.0, 1.0);
	float fogFactor = clamp((fogEnd - dist)/(fogEnd - fogStart), 0.0, 1.0);
	float furtherFogFactor = clamp((fogEnd - 0.85*dist)/(fogEnd - fogStart), 0.0, 1.0);
	float fogAlpha = mix(0.0f, 1.0f, furtherFogFactor); 
	//float fogAlpha = 1 - fadeFactor;
	vec2 localuv = fract(texCoords);
	vec3 d = abs(FragPos - uniformBreakPos); bvec3 inside = lessThanEqual(d, vec3(0.5 + 1e-4)); bool isBreakingBlock = all(inside);
	vec4 topTex = texture(topTexture, uvCoords);
	vec3 grassColor = ((uvCoords.y <= 0.2 && uvCoords.x <= 0.2)) ? mix(texture(grassColorTexture, FragPos.xz * 0.1), vec4(1), 0.5).rgb : vec3(1);
	vec4 overlayColorMask = ((uvCoords.y >= 0.4 && uvCoords.x >= 0.6) && (uvCoords.y <= 0.6 && uvCoords.x <= 0.8)) || ((uvCoords.y >= 0.6 && uvCoords.x >= 0.0) && (uvCoords.y <= 0.8 && uvCoords.x <= 0.2)) ? vec4(1.0f) : vec4(colorMask, 1.0f);
	vec4 breakStageTex = isBreakingBlock ? mix(texture(breakStageTexture, vec2(localuv.x + uniformBreaking, localuv.y) * 0.2), vec4(1), 0.25) : vec4(1);

	vec4 fragCol = (topTex * (vec4(colorMask, 1.0f) - vec4(0.3f, 0.0f, 0.1f, 0.0f)) + max((tex) * overlayColorMask - topTex * vec4(0.2f, 1.0f, 0.2f, 1.0f), vec4(0.2f))) * vec4(1.0, 1.0f, 1.0f, 1.0f) * finalColor;

	color = vec4(mix(vec3(0.6f, 0.7f, 0.85f), breakStageTex.rgb*fragCol.rgb, fogFactor), fogAlpha);
};