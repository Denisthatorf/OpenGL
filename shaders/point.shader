#shader vertex
#version 460

uniform mat4 u_MVP;

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _color;

out vec3 colorr;

void main(void){
    colorr = _color;
    gl_Position = u_MVP * vec4(_position,1.0);
}

#shader fragment
#version 460

in vec3 colorr;
out vec4 frag_color;

void main(void) {
    frag_color = vec4(colorr, 1.0);
}