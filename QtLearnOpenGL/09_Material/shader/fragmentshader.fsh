#version 330 core

varying vec3 M_Normal;
varying vec2 M_TexCoord;
varying vec3 M_ObjectPos;

uniform vec3 M_ViewPostion;

// 材质
struct Material
{
    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射
    vec3 specular;      // 镜面反射
    float shininess;    // 镜面发射系数
};

// 光
struct Light
{
    vec3 lightPos;      // 光源的位置

    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射
    vec3 specular;      // 镜面反射
};

uniform Material objectMaterial;        // 物体的材质
uniform Light lightMaterial;            // 光的信息

void main(void)
{
    // 环境光
    vec3 ambient = lightMaterial.ambient * objectMaterial.ambient;

    // 漫反射
    vec3 lightDir = normalize(lightMaterial.lightPos - M_ObjectPos);
    float diff = max(dot(M_Normal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial.diffuse * objectMaterial.diffuse;

    // 鏡面反射
    vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
    vec3 reflectDir = reflect(-lightDir, M_Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial.specular * objectMaterial.specular;

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
