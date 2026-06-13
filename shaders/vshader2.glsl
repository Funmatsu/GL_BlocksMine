#version 330 core                                                   
                                                                    
layout (location = 0) in vec3 pos;
                                                                    
uniform mat4 ortho;    
uniform mat4 model;     
                                                                    
void main(){                                                        
    	gl_Position = model * ortho * vec4(pos, 1.0);

}