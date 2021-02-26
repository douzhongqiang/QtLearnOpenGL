#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;
out vec3 M_OutColor;
in vec3 M_Color[];

in VertexAttribute
{
    vec4  pos;
} vertexAttr[];

void main(void)
{
    M_OutColor = M_Color[0];

    gl_Position = vertexAttr[0].pos + vec4(-0.2, 0.0, 0.0, 0.0); // 1:bottom-left
    EmitVertex();
    gl_Position = vertexAttr[0].pos + vec4(0.2, 0.0, 0.0, 0.0); // 2:bottom-right
    EmitVertex();

    EndPrimitive();
}
