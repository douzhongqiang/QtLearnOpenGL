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

// 是否显示场景
uniform bool M_isShowDepthTest;

// 将非线性的深度值转成线性的值
float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0;

    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main(void)
{
    if (!M_isShowDepthTest)
    {
        // 环境光
        vec3 ambient = lightMaterial.ambient * vec3(texture2D(objectMaterial.diffuse, M_TexCoord));

        // 漫反射
        vec3 lightDir = normalize(lightMaterial.direction - M_ObjectPos);
        float diff = max(dot(M_Normal, lightDir), 0);
        vec4 objectDiffuseTexture = texture2D(objectMaterial.diffuse, M_TexCoord);
        vec3 diffuse = diff * lightMaterial.diffuse * vec3(objectDiffuseTexture);

        // 鏡面反射
        vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
        vec3 reflectDir = normalize(reflect(-lightDir, M_Normal));
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
        vec3 specular = spec * lightMaterial.specular * vec3(texture2D(objectMaterial.specular, M_TexCoord));

//        gl_FragColor = vec4(ambient + diffuse + specular, objectDiffuseTexture.a);
            gl_FragColor = objectDiffuseTexture;
    }
    else
    {
        float depth = LinearizeDepth(gl_FragCoord.z);
        gl_FragColor = vec4(vec3(depth), 1.0);
    }
}
