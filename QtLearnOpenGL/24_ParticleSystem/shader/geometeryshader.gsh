#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in vec3 outPos[];
out vec3 outValue;

uniform float renderTime;

void processTpRect(vec3 pos)
{
    float interval = 0.1;

    // 左上
    outValue = vec3(pos.x - interval, pos.y + interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    // 左下
    outValue = vec3(pos.x - interval, pos.y - interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    // 右下
    outValue = vec3(pos.x + interval, pos.y - interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    // 左上
    outValue = vec3(pos.x - interval, pos.y + interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    // 右下
    outValue = vec3(pos.x + interval, pos.y - interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    // 右上
    outValue = vec3(pos.x + interval, pos.y + interval, pos.z);
//    gl_Position = vec4(outValue, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    float r = 0.8;
    float xPt = r * sin(renderTime);
    float yPt = r * cos(renderTime);

    vec3 resultPos = vec3(xPt, yPt, outPos[0].z);
    processTpRect(resultPos);
}
