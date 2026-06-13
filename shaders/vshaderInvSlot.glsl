#version 330 core                                                   
                                                                    
layout (location = 0) in vec3  pos;
layout (location = 1) in vec2  inTex;
layout (location = 2) in float frtscal;//Full Range Texture Scaling
layout (location = 3) in vec3  txrange;
layout (location = 4) in vec3  mask;

out vec3 colorMask;
out vec2 texCoords;
out vec2 texrange;
out float frts;
                                                                    
uniform mat4 ortho;
uniform mat4 model;    
                                                                    
void main(){                                                        
    gl_Position = ortho * model * vec4(pos, 1.0);
	texCoords = inTex;
    colorMask = mask;
	
	texrange = txrange.xy;
	frts = frtscal;
};