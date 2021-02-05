#ifndef COPENGLSKYBOX_H
#define COPENGLSKYBOX_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLShaderProgram>
#include "COpenGLCubeTexture.h"
#include "OpenGLEngine/COpenGLMesh.h"

class COpenGLSkyBox : public QObject
{
    Q_OBJECT

public:
    COpenGLSkyBox(QObject* parent = nullptr);
    ~COpenGLSkyBox();

    void init(void);

    // 设置CubeTexture
    void setCubeTexture(COpenGLCubeTexture* pTexture);
    // 绘制天空盒
    void draw(void);

private:
    COpenGLCubeTexture* m_pCubeTexture = nullptr;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    COpenGLMesh* m_pMesh = nullptr;

    bool initShaderProgram(void);
    void initMesh(void);
};

#endif
