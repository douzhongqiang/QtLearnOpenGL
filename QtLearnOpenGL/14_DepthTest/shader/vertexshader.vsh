#version 330 core

attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

varying vec3 M_Normal;
varying vec2 M_TexCoord;
varying vec3 M_ObjectPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(void)
{
    vec3 tempVec = mat3(transpose(inverse(M))) * normalize(normal);
    M_Normal = normalize(tempVec);
    M_TexCoord = texCoord * vec2(1.0, -1.0);
    M_ObjectPos = vec3(M * vec4(pos, 1.0));

    gl_Position = P * V * M * vec4(pos, 1.0);
}
