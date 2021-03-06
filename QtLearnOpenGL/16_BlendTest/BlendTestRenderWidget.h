#ifndef BLENDTESTRENDERWIDGET_H
#define BLENDTESTRENDERWIDGET_H

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
class BlendTestRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
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
    BlendTestRenderWidget(QWidget* parent = nullptr);
    ~BlendTestRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 设置/获取光的属性信息
    void setLightInfo(const LightInfo& info);
    LightInfo getLightInfo(void);

    void setCameraPostion(const QVector3D& postion);
    QVector3D getCameraPostion(void);

    void setCameraFront(const QVector3D& front);
    QVector3D getCameraFront(void);

    // 设置是否显示为深度测试结果
    void setDepthTestVisible(bool isVisible);
    bool isDepthTestVisible(void);

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
    bool initLightShaderProgram(void);
    bool initGrassShaderProgram(void);

    GLuint m_shaderProgramId;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    QOpenGLShaderProgram* m_pLightShaderProgram = nullptr;
    QOpenGLShaderProgram* m_pGrassShaderProgram = nullptr;
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
    COpenGLMesh* m_pMesh = nullptr;
    COpenGLMesh* m_pMeshFloor = nullptr;
    COpenGLMesh* m_pLightMesh = nullptr;
    COpenGLMesh* m_pTwoBoxMesh = nullptr;
    COpenGLMesh* m_pGrassMesh = nullptr;

    // 灯
    void initLight(QOpenGLFunctions* f);
    void drawLight(void);

    // 盒子
    void initBox(QOpenGLFunctions* f);
    void initModelData(COpenGLMesh* pMesh);
    void drawBox(const QVector3D& pos);
    void drawScaledBox(const QVector3D& pos);
    void drawTwoBox(void);
    void drawScaledTwoBox(void);

    // 地板
    void initFloor(QOpenGLFunctions* f);
    void initModelData2(void);      // 設置地板的数据
    void drawFloor(void);

    // 草
    void initGrass(QOpenGLFunctions* f);
    void initGrassModelData(COpenGLMesh* pMesh);
    void drawGrass(void);

    void initModelData3(void);

private:
    // 旋转
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    void onTimeout(void);
    int m_angle = 0;

    bool m_bShowDepthTest = false;

    QVector<QVector3D> m_grassPosVec;

signals:
    void attributeInfoChanged(void);
};

#endif
