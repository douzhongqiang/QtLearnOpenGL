attribute highp vec3 color;
attribute highp vec3 pos;

varying vec4 M_Color;

void main(void)
{
    gl_Position = vec4(pos, 1.0);
    M_Color = vec4(color, 1.0);
}
