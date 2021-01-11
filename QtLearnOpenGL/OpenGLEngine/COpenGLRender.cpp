#include "COpenGLRender.h"
#include "COpenGLTexture.h"
#include "COpenGLVertexObject.h"

COpenGLRender::COpenGLRender(QOpenGLFunctions* function, QObject* parent)
    :QObject (parent)
    , m_pFunction(function)
{

}

COpenGLRender::~COpenGLRender()
{

}

void COpenGLRender::setShaderProgram(QOpenGLShaderProgram* pProgram)
{
    m_pShaderProgram = pProgram;
}

void COpenGLRender::addTexture(COpenGLTexture* pTexture)
{
    if (pTexture)
        m_textures.append(pTexture);
}

void COpenGLRender::addVertexDataObject(COpenGLVertexObject* object)
{
    if (object)
    {
        m_pVertexObjects.append(object);
        object->setProgram(m_pShaderProgram);
    }
}

// 激活纹理
void COpenGLRender::activeTexture(void)
{
    int count = m_textures.size();
    for (int i=0; i<count; ++i)
    {
        // 设置0号纹理
        m_pFunction->glActiveTexture(GL_TEXTURE0 + i);
        m_pFunction->glBindTexture(GL_TEXTURE_2D, m_textures[i]->getId());

        QString textureName = m_textures[i]->getName();
        m_pShaderProgram->setUniformValue(textureName.toLocal8Bit().data(), i);
    }
}

void COpenGLRender::activeTexture(COpenGLTexture* pTexture)
{
    int count = m_textures.size();
    for (int i=0; i<count; ++i)
    {
        if (pTexture != m_textures[i])
            break;

        // 设置0号纹理
        m_pFunction->glActiveTexture(GL_TEXTURE0 + i);
        m_pFunction->glBindTexture(GL_TEXTURE_2D, m_textures[i]->getId());

        QString textureName = m_textures[i]->getName();
        m_pShaderProgram->setUniformValue(textureName.toLocal8Bit().data(), i);

        return;
    }
}

void COpenGLRender::activeVertex(void)
{
    for (auto iter = m_pVertexObjects.begin(); iter != m_pVertexObjects.end(); ++iter)
    {
        (*iter)->renderSelf();
    }
}

void COpenGLRender::init(void)
{
    for (auto iter = m_pVertexObjects.begin(); iter != m_pVertexObjects.end(); ++iter)
    {
        (*iter)->create();
    }
}

void COpenGLRender::render(void)
{
    // 设置纹理
    activeTexture();

    // 绘制顶点数据
    activeVertex();
}

void COpenGLRender::resize(int width, int height)
{
    m_pFunction->glViewport(0, 0, width, height);
}