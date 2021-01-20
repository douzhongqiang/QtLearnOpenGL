#include "COpenGLVertexObject.h"
#include "COpenGLVertexArray.h"
#include "COpenGLElementArray.h"

COpenGLVertexObject::COpenGLVertexObject(QOpenGLFunctions* function, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
    , m_type(t_Quads)
{
    m_pVertexArray = new COpenGLVertexArray(m_pFunction, this);
}

COpenGLVertexObject::~COpenGLVertexObject()
{

}

void COpenGLVertexObject::setPoints(const CAttributePointArray& points)
{
    m_points = points;
}

void COpenGLVertexObject::setIndices(const QVector<unsigned int> indices)
{
    m_indices = indices;
    if (m_pElementArray == nullptr && m_indices.size() > 0)
        m_pElementArray = new COpenGLElementArray(m_pFunction, this);
}

void COpenGLVertexObject::setProgram(QOpenGLShaderProgram* pProgram)
{
    m_pProgram = pProgram;
}

void COpenGLVertexObject::create(void)
{
    if (m_pElementArray)
    {
        std::function<void(void)> func = std::bind(&COpenGLVertexObject::createVBO, this);
        m_pElementArray->setCallbackFunction(func);

        m_pElementArray->create(m_indices.data(), m_indices.size() * sizeof(int));

        return;
    }

    createVBO();
}

void COpenGLVertexObject::bind(void)
{
    if (m_pElementArray)
    {
        m_pElementArray->bind();
        return;
    }

    m_pVertexArray->bind();
}

void COpenGLVertexObject::unbind(void)
{
    if (m_pElementArray)
    {
        m_pElementArray->unbind();
        return;
    }

    m_pVertexArray->unbind();
}

void COpenGLVertexObject::createVBO(void)
{
    m_pVertexArray->create(m_points.data(), m_points.size() * sizeof(CAttributePoint));
    m_pVertexArray->bind();

    // 设置顶点信息属性指针 - 3
    m_pFunction->glVertexAttribPointer(m_nPosAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(CAttributePoint), (void*)0);
    m_pFunction->glEnableVertexAttribArray(m_nPosAttrLocationId);
    // 设置原色信息属性指针 - 2
    m_pFunction->glVertexAttribPointer(m_nCoordAttrLocationId, 2, GL_FLOAT, GL_FALSE, sizeof(CAttributePoint), (void*)(sizeof (float) * 3));
    m_pFunction->glEnableVertexAttribArray(m_nCoordAttrLocationId);
    // 设置纹理坐标 - 3
    m_pFunction->glVertexAttribPointer(m_nNormalAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(CAttributePoint), (void*)(sizeof (float) * 5));
    m_pFunction->glEnableVertexAttribArray(m_nNormalAttrLocationId);

    m_pVertexArray->unbind();
}

void COpenGLVertexObject::setType(ObjectType type)
{
    m_type = type;
}

void COpenGLVertexObject::renderSelf(bool needBind)
{
    if (needBind)
        this->bind();

    GLenum type = GL_QUADS;
    if (m_type == t_Triangle)
        type = GL_TRIANGLES;

    // 绘制
    if (m_pElementArray)
        m_pFunction->glDrawElements(type, m_indices.size(), GL_UNSIGNED_INT, 0);
    else
        m_pFunction->glDrawArrays(type, 0, m_points.size());

    if (needBind)
        this->unbind();
}

// 设置和获取名字
void COpenGLVertexObject::setName(const QString& posName, const QString& coordName, const QString& normalName)
{
    m_posName = posName;
    m_coordName = coordName;
    m_normalName = normalName;

    m_nPosAttrLocationId = m_pProgram->attributeLocation(posName.toLocal8Bit().data());
    m_nCoordAttrLocationId = m_pProgram->attributeLocation(coordName.toLocal8Bit().data());
    m_nNormalAttrLocationId = m_pProgram->attributeLocation(normalName.toLocal8Bit().data());
}

void COpenGLVertexObject::getName(QString& posName, QString& coordName, QString& normalName)
{
    posName = m_posName;
    coordName = m_coordName;
    normalName = m_normalName;
}
