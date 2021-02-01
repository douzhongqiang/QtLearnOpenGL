#version 330 core

varying vec2 M_TexCoord;

// 材质
struct Material
{
    sampler2D diffuse;  // 漫反射贴图
    sampler2D specular;      // 镜面反射
    float shininess;    // 镜面发射系数
};

uniform Material objectMaterial;        // 物体的材质

void main(void)
{
    vec4 currentColor = texture2D(objectMaterial.diffuse, M_TexCoord);
    if (currentColor.a < 0.0001)
        discard;

    gl_FragColor = texture2D(objectMaterial.diffuse, M_TexCoord);
}
