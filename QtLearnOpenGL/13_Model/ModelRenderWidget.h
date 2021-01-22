#ifndef MODELRENDERWIDGET_H
#define MODELRENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>

class COpenGLCamera;
class COpenGLModel;
class COpenGLMesh;
class ModelRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    struct LightInfo
    {
        QVector3D lightPos;

        QVector3D ambientColor;         // 环境光颜色 - 物体的颜色
        QVector3D diffuesColor;         // 漫反射 - 物体的颜色
        QVector3D specularColor;        // 镜面反射颜色
    };

public:
    ModelRenderWidget(QWidget* parent = nullptr);
    ~ModelRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 设置/获取光的属性信息
    void setLightInfo(const LightInfo& info);
    LightInfo getLightInfo(void);

    // 设置模型路径
    void setModel(const QString& path);

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

    GLuint m_shaderProgramId;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    QOpenGLShader* m_pVertexShader = nullptr;
    QOpenGLShader* m_pFragmentShader = nullptr;

    QMatrix4x4 m_MMat;

    bool m_isFill = true;

private:
    // 光
    LightInfo m_light;
    void initLightInfo(void);

private:
    COpenGLCamera* m_pCamera = nullptr;
    COpenGLModel* m_pModel = nullptr;
    COpenGLMesh* m_pMesh = nullptr;

    void initModelData(void);

private:
    // 旋转
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    void onTimeout(void);
    int m_angle = 0;
};

#endif
