#include "COpenGLVertexObject.h"
#include "COpenGLVertexArray.h"

COpenGLVertexObject::COpenGLVertexObject(QOpenGLFunctions* function, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
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

void COpenGLVertexObject::setProgram(QOpenGLShaderProgram* pProgram)
{
    m_pProgram = pProgram;
}

void COpenGLVertexObject::create(void)
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

void COpenGLVertexObject::renderSelf(void)
{
    m_pVertexArray->bind();
    // 绘制
    m_pFunction->glDrawArrays(GL_QUADS, 0, m_points.size());

    m_pVertexArray->unbind();
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
