attribute highp vec3 color;
attribute highp vec3 pos;
attribute highp vec2 texCoord;

varying vec4 M_Color;
varying vec2 M_TexCoord;

void main(void)
{
    gl_Position = vec4(pos, 1.0);
    M_Color = vec4(color, 1.0);
    M_TexCoord = texCoord;
}
