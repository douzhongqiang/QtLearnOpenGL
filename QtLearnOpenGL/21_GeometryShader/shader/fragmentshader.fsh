#version 330 core

out vec4 FragColor;
in vec3 M_OutColor;

void main(void)
{
    FragColor = vec4(M_OutColor, 1.0);
}
