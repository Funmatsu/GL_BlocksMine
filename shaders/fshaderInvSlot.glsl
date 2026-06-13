#version 330 core
                                       
out vec4 color;
uniform sampler2D theTexture;
in vec2 texCoords;
in vec3 colorMask;

in vec2 texrange;
in float frts;

void main(){
	vec2 texuvs;
	if(frts > 0.5)
		texuvs = texCoords;
	else
		texuvs = clamp(fract(texCoords), texrange.x, texrange.y);
    color = texture(theTexture, texuvs) * vec4(colorMask, 1.0f);
};