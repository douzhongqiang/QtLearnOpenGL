#version 330 core

varying vec3 M_Normal;
varying vec2 M_TexCoord;
varying vec3 M_ObjectPos;

uniform vec3 M_ViewPostion;

// 材质
struct Material
{
    bool usedTexture;       // True使用贴图， false不使用

    sampler2D diffuse;      // 漫反射贴图
    sampler2D specular;     // 镜面反射

    vec3 ambientColor;      // 环境光颜色
    vec3 diffuseColor;      // 漫反射颜色
    vec3 specularColor;     // 镜面反射颜色

    float shininess;        // 镜面发射系数
};

// 光
struct Light
{
    bool enabled;       // 使能

    vec3 direction;      // 光源的位置

    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射
    vec3 specular;      // 镜面反射
};

uniform Material objectMaterial;        // 物体的材质
uniform Light lightMaterial;            // 光的信息
uniform vec3 objectFactor;

void main(void)
{
    if (lightMaterial.enabled)
    {
        // 环境光
        vec3 ambient;
        if (objectMaterial.usedTexture)
            ambient = lightMaterial.ambient * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));
        else
            ambient = lightMaterial.ambient * objectMaterial.ambientColor;

        // 漫反射
        vec3 lightDir = normalize(lightMaterial.direction - M_ObjectPos);
        float diff = max(dot(M_Normal, lightDir), 0);
        vec3 diffuse;
        if (objectMaterial.usedTexture)
            diffuse = diff * lightMaterial.diffuse * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));
        else
            diffuse = diff * lightMaterial.diffuse * objectMaterial.ambientColor;

        // 鏡面反射
        vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
        vec3 reflectDir = normalize(reflect(-lightDir, M_Normal));
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
        vec3 specular;
        if (objectMaterial.usedTexture)
            specular = spec * lightMaterial.specular * vec3(texture2D(objectMaterial.specular, M_TexCoord));
        else
            specular = spec * lightMaterial.specular * objectMaterial.specularColor;

        gl_FragColor = vec4(objectFactor * (ambient + diffuse + specular), 1.0);
    }
    else
    {
        // 环境光
        vec3 ambient;
        if (objectMaterial.usedTexture)
            ambient = lightMaterial.ambient * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));
        else
            ambient = lightMaterial.ambient * objectMaterial.ambientColor;

        // 漫反射
        vec3 diffuse;
        if (objectMaterial.usedTexture)
            diffuse = lightMaterial.diffuse * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));
        else
            diffuse = lightMaterial.diffuse * objectMaterial.ambientColor;

        gl_FragColor = vec4((ambient + diffuse) * objectFactor, 1.0);
    }
}
