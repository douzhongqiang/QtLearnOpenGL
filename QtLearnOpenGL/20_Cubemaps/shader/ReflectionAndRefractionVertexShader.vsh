#version 330 core

attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
varying vec3 M_Pos;
varying vec3 M_Normal;

void main(void)
{
    M_Pos = vec3(M * vec4(pos, 1.0));
    vec3 tempVec = mat3(transpose(inverse(M))) * normalize(normal);
    M_Normal = normalize(tempVec);

    gl_Position = P * V * M * vec4(pos, 1.0);
}
