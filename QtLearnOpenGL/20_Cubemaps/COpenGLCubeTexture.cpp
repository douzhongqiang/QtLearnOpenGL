#include "COpenGLCubeTexture.h"
#include <QImage>

COpenGLCubeTexture::COpenGLCubeTexture(QObject* parent)
    :QObject(parent)
{

}

COpenGLCubeTexture::~COpenGLCubeTexture()
{

}

void COpenGLCubeTexture::create(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glGenTextures(1, &textureId);

    this->bind();
    int count = 0;
    for (auto iter = m_cubeTextureMap.begin(); iter != m_cubeTextureMap.end(); ++iter)
    {
        QImage tempImage = iter.value();
        if (tempImage.format() != QImage::Format_RGB888)
            tempImage = tempImage.convertToFormat(QImage::Format_RGB888);

        pFunc->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count++, 0, GL_RGB, \
                            tempImage.width(), tempImage.height(), 0, GL_RGB, \
                            GL_UNSIGNED_BYTE, tempImage.constBits());
    }

    pFunc->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    pFunc->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    pFunc->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    pFunc->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    pFunc->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    this->unbind();
}

void COpenGLCubeTexture::bind(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

void COpenGLCubeTexture::unbind(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void COpenGLCubeTexture::setImage(CubeTextureType type, const QImage& image)
{
    m_cubeTextureMap.insert(type, image);
}

GLuint COpenGLCubeTexture::getTextureId(void)
{
    return textureId;
}
