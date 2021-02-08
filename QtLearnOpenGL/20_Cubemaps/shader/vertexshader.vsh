#version 330 core

attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

uniform mat4 V;
uniform mat4 P;
varying vec3 M_Pos;

void main(void)
{
    M_Pos = pos;

    gl_Position = P * V * vec4(pos, 1.0);
}
