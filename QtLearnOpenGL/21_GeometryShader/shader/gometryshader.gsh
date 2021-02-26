#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
out vec3 M_OutColor;
in vec3 M_Color[];

in VertexAttribute
{
    vec4  pos;
} vertexAttr[];

void build_house(vec4 position)
{
    M_OutColor = M_Color[0];
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top
    M_OutColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main(void)
{
    build_house(vertexAttr[0].pos);
}
