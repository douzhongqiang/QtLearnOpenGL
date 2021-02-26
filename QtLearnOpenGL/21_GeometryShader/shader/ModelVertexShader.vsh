#version 330 core

attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

out VertexAttribute
{
    vec3    pos;
    vec2    texCoord;
    vec3    normal;
} vs_out;

out vec4 OutPos;
out vec4 OutNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(void)
{
    gl_Position = P * V * M * vec4(pos, 1.0);

    OutPos = gl_Position;

    vs_out.normal = mat3(transpose(inverse(M))) * normalize(normal);
    vs_out.texCoord = texCoord * vec2(1.0, -1.0);
    vs_out.pos = vec3(M * vec4(pos, 1.0));

    OutNormal = P * transpose(inverse(V * M)) * normalize(vec4(normal, 0.0));
}
