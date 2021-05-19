#version 330 core

layout (location = 0) in vec3 pos;

out vec3 outPos;

void main(void)
{
    outPos = pos;

//    gl_Position = vec4(pos, 1.0);
//    gl_PointSize = 0.2;
}
