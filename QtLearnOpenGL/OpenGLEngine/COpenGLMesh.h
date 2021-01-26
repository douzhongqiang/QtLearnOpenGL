#ifndef COPENGLMESH_H
#define COPENGLMESH_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include "OpenGLEngine/COpenGLElementArray.h"
#include "OpenGLEngine/COpenGLVertexArray.h"
#include "OpenGLEngine/COpenGLVertexObject.h"
#include "OpenGLEngine/COpenGLTexture.h"
#include "OpenGLEngine/CAttributePoint.h"

class COpenGLMesh : public QObject
{
    Q_OBJECT

public:
    COpenGLMesh(QOpenGLFunctions* function, QOpenGLShaderProgram* pProgram, QObject* parent = nullptr);
    ~COpenGLMesh();

    void setPoints(const CAttributePointArray& points);
    void setIndices(const QVector<unsigned int> indices);
    void addTexture(COpenGLTexture* texture);
    void setObjectType(COpenGLVertexObject::ObjectType type);

    void setupMesh(void);

    void draw(void);

private:
    COpenGLVertexObject* m_pObject = nullptr;
    QVector<COpenGLTexture*> m_textures;

    QOpenGLFunctions* m_pFunction = nullptr;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    void activeTexture(void);

    GLuint m_VAO;
};

#endif
