#include "COpenGLFrameBufferObject.h"
#include <QDebug>

COpenGLFrameBufferObject::COpenGLFrameBufferObject(int width, int height, QObject* parent)
    :QObject(parent)
    , m_nWidth(width)
    , m_nHeight(height)
{

}

COpenGLFrameBufferObject::~COpenGLFrameBufferObject()
{
//    release();
}

bool COpenGLFrameBufferObject::create(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glGenFramebuffers(1, &m_frameBufferObjectId);
    this->bind();

    // 创建纹理
    pFunc->glGenTextures(1, &m_textureId);
    pFunc->glBindTexture(GL_TEXTURE_2D, m_textureId);
    pFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    pFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    pFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    pFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    pFunc->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // 添加纹理附件到FBO
    pFunc->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

    // 创建RBO(Render Buffer Object)
    pFunc->glGenRenderbuffers(1, &m_rboId);
    pFunc->glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
    pFunc->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_nWidth, m_nHeight);
    // 添加RBO附件到FBO
    pFunc->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

    m_isSuccessed = pFunc->glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    qDebug() << m_isSuccessed;
    if (m_isSuccessed)
        unbind();

    return m_isSuccessed;
}

void COpenGLFrameBufferObject::bind(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObjectId);
}

void COpenGLFrameBufferObject::unbind(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void COpenGLFrameBufferObject::release(void)
{
    if (!m_isSuccessed)
        return;

    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->glDeleteFramebuffers(1, &m_frameBufferObjectId);
}

GLuint COpenGLFrameBufferObject::getTextureId(void)
{
    return m_textureId;
}

int COpenGLFrameBufferObject::width(void)
{
    return m_nWidth;
}

int COpenGLFrameBufferObject::height(void)
{
    return m_nHeight;
}
