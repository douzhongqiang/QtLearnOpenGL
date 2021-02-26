#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;
out vec3 M_OutColor;
in vec3 M_Color[];

in VertexAttribute
{
    vec3    pos;
    vec2    texCoord;
    vec3    normal;
} gs_in[];

out VertexAttribute
{
    vec3    pos;
    vec2    texCoord;
    vec3    normal;
} gs_out;

in vec4 OutPos[];
uniform float time;

vec3 GetNormal()
{
   vec3 a = vec3(OutPos[0]) - vec3(OutPos[1]);
   vec3 b = vec3(OutPos[2]) - vec3(OutPos[1]);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

const float MAGNITUDE = 0.4;
in vec4 OutNormal[];
void GenerateLine(int index)
{
    gl_Position = OutPos[index];
    EmitVertex();

    gl_Position = OutPos[index] + OutNormal[index] * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main(void)
{
    vec3 normal = GetNormal();

    for (int i=0; i<3; ++i)
    {
        gs_out.pos = gs_in[i].pos;
        gs_out.texCoord = gs_in[i].texCoord;
        gs_out.normal = gs_in[i].normal;

        gl_Position = OutPos[i];
        EmitVertex();
    }

    EndPrimitive();
}
