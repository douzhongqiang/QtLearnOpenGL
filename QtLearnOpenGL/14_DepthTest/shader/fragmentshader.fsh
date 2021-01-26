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
    vec3 direction;      // 光源的位置

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

    // 漫反射
    vec3 lightDir = normalize(-lightMaterial.direction);
    float diff = max(dot(M_Normal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial.diffuse * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));

    // 鏡面反射
    vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
    vec3 reflectDir = reflect(-lightDir, M_Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial.specular * vec3(texture2D(objectMaterial.specular, M_TexCoord));

//    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
    gl_FragColor = vec4(M_Normal, 1.0);
//    gl_FragColor = texture2D(objectMaterial.diffuse, M_TexCoord);
}
