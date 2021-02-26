#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out VertexAttribute
{
    vec4  pos;
} vertexAttr;

out vec3 M_Color;

void main(void)
{
    gl_Position = vec4(pos, 1.0);

    vertexAttr.pos = gl_Position;
    M_Color = color;
}
