#version 330 core

uniform vec3 M_Color;

void main(void)
{
    gl_FragColor = vec4(M_Color, 1.0);
}
