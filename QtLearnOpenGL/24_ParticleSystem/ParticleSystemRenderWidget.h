#ifndef PARTICLESYSTEMRENDERWIDGET_H
#define PARTICLESYSTEMRENDERWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QTime>
#include <QTimer>

class ParticleSystemRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_4_4_Core
{
    Q_OBJECT

public:
    ParticleSystemRenderWidget(QWidget* parent = nullptr);
    ~ParticleSystemRenderWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void updateData(void);

private:
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    bool initShaderProgram(void);

    GLuint m_translateFeedback[2];
    GLuint m_vboBufferId[2];

    int m_nCurrentRenderIndex = 1;
    int m_nCurrentTFIndex = 0;

    QVector3D m_myTestPos;
    QVector<QVector3D> m_pos;
    QTime m_time;
    QTimer* m_pTimer = nullptr;

    GLuint m_nVBO;
};

#endif
