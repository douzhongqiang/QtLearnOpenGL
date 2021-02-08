#version 330 core

uniform samplerCube skyBoxCube;
varying vec3 M_Pos;

void main(void)
{
    gl_FragColor = textureCube(skyBoxCube, M_Pos);
}
