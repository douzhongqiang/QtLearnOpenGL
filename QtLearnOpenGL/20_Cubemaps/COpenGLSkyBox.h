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
#include "OpenGLEngine/COpenGLCamera.h"

class COpenGLSkyBox : public QObject
{
    Q_OBJECT

public:
    COpenGLSkyBox(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLSkyBox();

    void init(void);

    // 设置CubeTexture
    void setCubeTexture(COpenGLCubeTexture* pTexture);

    // 设置相机
    void setCamera(COpenGLCamera* pCamera);

    // 绘制天空盒
    void draw(void);

private:
    COpenGLCubeTexture* m_pCubeTexture = nullptr;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    COpenGLCamera* m_pCamera = nullptr;
    COpenGLMesh* m_pMesh = nullptr;
    QOpenGLFunctions* m_pFunction = nullptr;

    bool initShaderProgram(void);
    void initMesh(void);
};

#endif
