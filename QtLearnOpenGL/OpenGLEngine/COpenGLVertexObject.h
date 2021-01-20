#ifndef COPENGLVERTEXOBJECT_H
#define COPENGLVERTEXOBJECT_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "CAttributePoint.h"

class COpenGLVertexArray;
class COpenGLElementArray;
class COpenGLVertexObject : public QObject
{
    Q_OBJECT
public:
    enum ObjectType
    {
        t_Triangle,
        t_Quads
    };

public:
    COpenGLVertexObject(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLVertexObject();

    void setPoints(const CAttributePointArray& points);
    void setIndices(const QVector<unsigned int> indices);
    void setProgram(QOpenGLShaderProgram* pProgram);

    // 设置和获取名字
    void setName(const QString& posName, const QString& coordName, const QString& normalName);
    void getName(QString& posName, QString& coordName, QString& normalName);

    void create(void);
    void bind(void);
    void unbind(void);

    void setType(ObjectType type);

    void renderSelf(bool needBind = true);

private:
    QOpenGLFunctions* m_pFunction = nullptr;

    CAttributePointArray m_points;
    COpenGLVertexArray* m_pVertexArray = nullptr;
    COpenGLElementArray* m_pElementArray = nullptr;
    QOpenGLShaderProgram* m_pProgram = nullptr;

    QVector<unsigned int> m_indices;

    GLint m_nPosAttrLocationId;
    GLint m_nCoordAttrLocationId;
    GLint m_nNormalAttrLocationId;

    QString m_posName;
    QString m_coordName;
    QString m_normalName;

    ObjectType m_type;

    void createVBO(void);
};

#endif
