#ifndef DRAWINSTANCERENDERWIDGET_H
#define DRAWINSTANCERENDERWIDGET_H

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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DrawInstaceRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    DrawInstaceRenderWidget(QWidget* parent = nullptr);
    ~DrawInstaceRenderWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool initShaderProgram(void);
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    void initPoints(void);
    GLuint m_nVBO;

    void initMatrixs(void);
    QVector<glm::mat4> m_matrixVecs;
    QVector<GLuint> m_VAOVecs;
    int m_nTotalSize = 1000;
    GLuint m_nVBOId; 
};

#endif
