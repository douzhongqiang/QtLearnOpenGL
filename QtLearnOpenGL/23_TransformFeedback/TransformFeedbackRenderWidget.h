#ifndef TRANSFORMFEEDBACKRENDERWIDGET_H
#define TRANSFORMFEEDBACKRENDERWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

class TransformFeedbackRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    TransformFeedbackRenderWidget(QWidget* parent = nullptr);
    ~TransformFeedbackRenderWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool initShaderProgram(void);
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_tbo;

    GLuint m_queryId;
};

#endif
