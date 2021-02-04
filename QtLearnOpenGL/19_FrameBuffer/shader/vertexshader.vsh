#version 330 core

attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

varying vec2 M_TexCoord;

void main(void)
{
    gl_Position = vec4(pos, 1.0);
    M_TexCoord = texCoord;
}
