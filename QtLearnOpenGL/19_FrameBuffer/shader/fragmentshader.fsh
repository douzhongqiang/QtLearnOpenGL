#version 330 core

varying vec2 M_TexCoord;
uniform sampler2D fboImage;

void main(void)
{
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = texture2D(fboImage, M_TexCoord);
}
