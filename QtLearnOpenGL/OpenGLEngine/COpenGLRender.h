#ifndef COPENGLRENDER_H
#define COPENGLRENDER_H

#include <QOpenGLFunctions>
#include <QObject>
#include <QOpenGLShaderProgram>
#include "CAttributePoint.h"

class COpenGLTexture;
class COpenGLVertexObject;
class COpenGLRender : public QObject
{
    Q_OBJECT

public:
    COpenGLRender(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLRender();

public:
    void init(void);
    void render(void);
    void resize(int width, int height);

public:

    // 设置
    void setShaderProgram(QOpenGLShaderProgram* pProgram);

    // 添加Texture
    void addTexture(COpenGLTexture* pTexture);

    // 设置顶点数据
    void addVertexDataObject(COpenGLVertexObject* object);

    // 激活纹理
    void activeTexture(void);
    void activeTexture(COpenGLTexture* pTexture);

    // 渲染顶点数据
    void activeVertex(void);

private:
    QVector<COpenGLTexture*> m_textures;
    QOpenGLFunctions* m_pFunction = nullptr;

    QVector<COpenGLVertexObject*> m_pVertexObjects;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
};

#endif
