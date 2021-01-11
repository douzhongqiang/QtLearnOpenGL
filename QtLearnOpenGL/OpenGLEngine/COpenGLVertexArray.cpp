#include "COpenGLVertexArray.h"

COpenGLVertexArray::COpenGLVertexArray(QOpenGLFunctions* function, QObject* parent)
    :QObject (parent)
    , m_pFunction(function)
{

}

COpenGLVertexArray::~COpenGLVertexArray()
{

}

// 创建
void COpenGLVertexArray::create(void* pData, int size)
{
    m_pFunction->glGenBuffers(1, &m_id);
    bind();

    m_pFunction->glBufferData(GL_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW);

    unbind();
}

// 绑定
void COpenGLVertexArray::bind(void)
{
    m_pFunction->glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void COpenGLVertexArray::unbind(void)
{
    m_pFunction->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 获取id
GLuint COpenGLVertexArray::getId(void)
{
    return m_id;
}
