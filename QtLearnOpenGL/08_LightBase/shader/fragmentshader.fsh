#version 330 core

varying vec3 M_Normal;
varying vec2 M_TexCoord;
varying vec3 M_ObjectPos;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float mixNumber;        // [0, 1]
uniform vec3 M_ObjecColor;
uniform vec3 M_LightColor;
uniform vec3 M_LightPostion;
uniform vec3 M_ViewPostion;

void main(void)
{
    //gl_FragColor = mix(texture2D(texture1, M_TexCoord * vec2(1.0, -1.0)), texture2D(texture2, M_TexCoord * vec2(1.0, -1.0)), mixNumber);
    //gl_FragColor = vec4(M_ObjecColor, 1.0);

    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * M_LightColor;

    // 漫反射
    vec3 lightDir = normalize(M_LightPostion - M_ObjectPos);
    float diff = max(dot(M_Normal, lightDir), 0);
    vec3 diffuse = diff * M_LightColor;

    // 鏡面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(M_ViewPostion - M_ObjectPos);
    vec3 reflectDir = reflect(-lightDir, M_Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * M_LightColor;

    gl_FragColor = vec4((ambient + diffuse + specular) * M_ObjecColor, 1.0);
}
