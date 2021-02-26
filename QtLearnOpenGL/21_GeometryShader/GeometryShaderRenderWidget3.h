#ifndef GEOMETRYSHADERRENDERWIDGET3_H
#define GEOMETRYSHADERRENDERWIDGET3_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>
#include <QTime>
#include "OpenGLEngine/COpenGLFrameBufferObject.h"
#include "OpenGLEngine/COpenGLModel.h"
#include "OpenGLEngine/COpenGLMesh.h"
#include "OpenGLEngine/COpenGLCamera.h"

class GeometryShaderRenderWidget3 : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GeometryShaderRenderWidget3(QWidget* parent = nullptr);
    ~GeometryShaderRenderWidget3();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool initShaderProgram(void);
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    void initPoints(void);
    GLuint m_nVBO;

    void initModel(void);
    void initCamera(void);

private:
    COpenGLModel* m_pModel = nullptr;
    COpenGLCamera* m_pCamera = nullptr;

    // 旋转
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    void onTimeout(void);
    int m_angle = 0;
    QTime m_time;
};

#endif
