#version 330 core

varying vec3 M_Normal;
varying vec2 M_TexCoord;
varying vec3 M_ObjectPos;

uniform vec3 M_ViewPostion;

// 材质
struct Material
{
    sampler2D diffuse;  // 漫反射贴图
    sampler2D specular;      // 镜面反射
    float shininess;    // 镜面发射系数
};

// 光
struct Light
{
    vec3 lightPos;      // 光源的位置
    vec3 direction;     // 方向
    float cutoff;       // 聚光灯的切角
    float outerCutoff;    // 外圆锥的切角

    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射
    vec3 specular;      // 镜面反射
};

uniform Material objectMaterial;        // 物体的材质
uniform Light lightMaterial;            // 光的信息

void main(void)
{
    // 环境光
    vec3 ambient = lightMaterial.ambient * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));

    vec3 lightDir = normalize(lightMaterial.lightPos - M_ObjectPos);
    float theta = dot(lightDir, normalize(-lightMaterial.direction));
    float epsilon = lightMaterial.cutoff - lightMaterial.outerCutoff;
    float intensity = clamp((theta - lightMaterial.outerCutoff) / epsilon, 0.0, 1.0);

    // 漫反射
    float diff = max(dot(M_Normal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial.diffuse * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));
    diffuse *= intensity;

    // 鏡面反射
    vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
    vec3 reflectDir = reflect(-lightDir, M_Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial.specular * vec3(texture2D(objectMaterial.specular, M_TexCoord));
    specular *= intensity;

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
