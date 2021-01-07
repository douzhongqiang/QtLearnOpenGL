#version 330 core

varying vec4 M_Color;
varying vec2 M_TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixNumber;        // [0, 1]

void main(void)
{
    gl_FragColor = mix(texture2D(texture1, M_TexCoord * vec2(1.0, -1.0)), texture2D(texture2, M_TexCoord * vec2(1.0, -1.0)), mixNumber);
}
