attribute highp vec3 pos;
attribute highp vec2 texCoord;
attribute highp vec3 normal;

varying vec3 M_Normal;
varying vec2 M_TexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(void)
{
    gl_Position = P * V * M * vec4(pos, 1.0);

    M_Normal = normal;
    M_TexCoord = texCoord;
}
