#include "COpenGLTexture.h"
#include <QImage>

COpenGLTexture::COpenGLTexture(QOpenGLFunctions* function, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
    , m_filterType(t_linear)
{

}

COpenGLTexture::~COpenGLTexture()
{

}

// 创建纹理
void COpenGLTexture::create(void)
{
    m_pFunction->glGenTextures(1, &m_id);
    m_pFunction->glBindTexture(GL_TEXTURE_2D, m_id);

    m_pFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_pFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint filterType = GL_NEAREST;
    if (m_filterType == t_linear)
        filterType = GL_LINEAR;
    m_pFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
    m_pFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);

    m_pFunction->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_pFunction->glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    m_pFunction->glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    m_pFunction->glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    m_pFunction->glBindTexture(GL_TEXTURE_2D, 0);
}

// 绑定
void COpenGLTexture::bind(void)
{
    m_pFunction->glBindTexture(GL_TEXTURE_2D, m_id);
}

void COpenGLTexture::unbind(void)
{
    m_pFunction->glBindTexture(GL_TEXTURE_2D, 0);
}

// 设置图片
void COpenGLTexture::setImage(const QImage& image)
{
    this->bind();

    QImage tempImage = image;
    if (tempImage.format() != QImage::Format_RGBA8888)
        tempImage = tempImage.convertToFormat(QImage::Format_RGBA8888);

    m_pFunction->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempImage.width(), tempImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tempImage.bits());

    this->unbind();
}

void COpenGLTexture::setImage(const QString& image)
{
    m_imagePath = image;
    QImage tempImage;
    if (!tempImage.load(image))
        return;

    this->setImage(tempImage);
}

// 设置/获取纹理的名字 - shader中的名字
void COpenGLTexture::setName(const QString& name)
{
    m_name = name;
}

QString COpenGLTexture::getName(void)
{
    return m_name;
}

void COpenGLTexture::setType(TextureType type)
{
    m_type = type;
}

COpenGLTexture::TextureType COpenGLTexture::getType(void)
{
    return m_type;
}

void COpenGLTexture::setFilterType(FilterType type)
{
    m_filterType = type;
}

COpenGLTexture::FilterType COpenGLTexture::getFilterType(void)
{
    return m_filterType;
}

// 获取ID
GLuint COpenGLTexture::getId(void)
{
    return m_id;
}

// 获取图片的路径
QString COpenGLTexture::getImagePath(void)
{
    return m_imagePath;
}
