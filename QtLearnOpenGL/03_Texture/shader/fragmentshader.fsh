varying vec4 M_Color;
varying vec2 M_TexCoord;

uniform sampler2D texture1;

void main(void)
{
    gl_FragColor = texture2D(texture1, M_TexCoord) * M_Color;
}
