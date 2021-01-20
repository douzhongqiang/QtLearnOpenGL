#include "COpenGLElementArray.h"

COpenGLElementArray::COpenGLElementArray(QOpenGLFunctions* function, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
{

}

COpenGLElementArray::~COpenGLElementArray()
{

}

// 创建
void COpenGLElementArray::create(void* pData, int size)
{
    m_pFunction->glGenBuffers(1, &m_id);
    bind();

    m_pFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW);

    if (m_fun)
        m_fun();

    unbind();
}

// 绑定
void COpenGLElementArray::bind(void)
{
    m_pFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void COpenGLElementArray::unbind(void)
{
    m_pFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// 获取id
GLuint COpenGLElementArray::getId(void)
{
    return m_id;
}

void COpenGLElementArray::setCallbackFunction(std::function<void(void)> func)
{
    m_fun = func;
}
