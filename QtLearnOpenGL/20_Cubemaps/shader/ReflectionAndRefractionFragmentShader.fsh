#version 330 core

varying vec3 M_Pos;
varying vec3 M_Normal;

uniform samplerCube skyBoxCube;
uniform vec3 M_EyePostion;
uniform bool M_isRefection;

void main(void)
{
    float ratio = 1.00 / 1.52;

    vec3 InputVec = normalize(M_Pos - M_EyePostion);    // 入射光线
    vec3 refectionVec = reflect(InputVec, M_Normal);    // 反射光线
    vec3 refractionVec = refract(InputVec, M_Normal, ratio);    // 折射光线

    if (M_isRefection)
        gl_FragColor = vec4(textureCube(skyBoxCube, refectionVec).rgb, 1.0);
    else
        gl_FragColor = vec4(textureCube(skyBoxCube, refractionVec).rgb, 1.0);
}
