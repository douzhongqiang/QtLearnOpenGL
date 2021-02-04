#version 330 core

varying vec2 M_TexCoord;
uniform sampler2D fboImage;

uniform int M_Type;
uniform mat3 M_kernel;

// 卷积计算后的颜色数据
vec3 disposeCpmvolutionKernel(sampler2D image, vec2 coord, mat3 kernel)
{
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // 左上
            vec2( 0.0f,    offset), // 正上
            vec2( offset,  offset), // 右上
            vec2(-offset,  0.0f),   // 左
            vec2( 0.0f,    0.0f),   // 中
            vec2( offset,  0.0f),   // 右
            vec2(-offset, -offset), // 左下
            vec2( 0.0f,   -offset), // 正下
            vec2( offset, -offset)  // 右下
        );

    vec3 destColor = vec3(0.0);
    for (int i=0; i<9; ++i)
    {
        vec3 tempColor = vec3(texture2D(image, coord + offsets[i]));
        destColor += kernel[i / 3][i % 3] * tempColor;
    }

    return destColor;
}

void main(void)
{
    switch (M_Type)
    {
    case 0:
        gl_FragColor = texture2D(fboImage, M_TexCoord);
        break;
    case 1:
        gl_FragColor = vec4(vec3(1.0 - texture2D(fboImage, M_TexCoord)), 1.0);
        break;
    case 2:
    {
        vec3 tempColor = vec3(texture2D(fboImage, M_TexCoord));
        float average = 0.2126 * tempColor.r + 0.7152 * tempColor.g + 0.0722 * tempColor.b;
        gl_FragColor = vec4(average, average, average, 1.0);
        break;
    }
    case 3:
    {
        gl_FragColor = vec4(disposeCpmvolutionKernel(fboImage, M_TexCoord, M_kernel), 1.0);
        break;
    }
    default:
        gl_FragColor = texture2D(fboImage, M_TexCoord);
    }
}
