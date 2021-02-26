#ifndef GEOMETRYSHADERRENDERWIDGET_H
#define GEOMETRYSHADERRENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>
#include "OpenGLEngine/COpenGLFrameBufferObject.h"
#include "OpenGLEngine/COpenGLModel.h"
#include "OpenGLEngine/COpenGLMesh.h"

class GeometryShaderRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GeometryShaderRenderWidget(QWidget* parent = nullptr);
    ~GeometryShaderRenderWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool initShaderProgram(void);
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    void initPoints(void);
    GLuint m_nVBO;
};

#endif
