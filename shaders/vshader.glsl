#version 430 core                                                   
                                                                    
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTex;
layout (location = 2) in float transparencyIn;
layout (location = 3) in float normcol;

out vec3 normal;

out vec4 pdc;
out vec2 texCoords;
flat out float transparency;
out vec3 FragPos;
out vec3 colorMask;
                                                       
uniform mat4 model;                                                 
uniform mat4 projection;
uniform mat4 view;     
uniform mat4 directionalLightTransform;         
out vec4 directionalLightSpacePos;     
                                                                    
void main(){                                                        
    gl_Position = projection * view * model * vec4(pos, 1.0);
	pdc = vec4(pos, 1.0);
	
	directionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
	texCoords = inTex;
	transparency = transparencyIn;
	uint unormcol = floatBitsToUint(normcol);

	int normfactorx = -(int((unormcol >> 5u) & 1u) + 1)/2, 
		normfactory = -(int((unormcol >> 3u) & 1u) + 1)/2,
		normfactorz = -(int((unormcol >> 1u) & 1u) + 1)/2;
	int nx = normfactorx * int((unormcol >> 4u) & 1u),
		ny = normfactory * int((unormcol >> 2u) & 1u),
		nz = normfactorz * int((unormcol >> 0u) & 1u);

	normal = normalize(mat3(transpose(inverse(model))) * vec3(nx, ny, nz));
	FragPos = (model * vec4(pos, 1.0)).xyz;
	int cx = int((unormcol >> 20u) & 0x7Fu),
		cy = int((unormcol >> 13u) & 0x7Fu),
		cz = int((unormcol >>  6u) & 0x7Fu);
	colorMask = vec3(cx, cy, cz)/100;
};