#ifndef CUBEMAPSRENDERWIDGET_H
#define CUBEMAPSRENDERWIDGET_H

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

class COpenGLCamera;
class COpenGLModel;
class COpenGLMesh;
class COpenGLCubeTexture;
class COpenGLSkyBox;
class CubemapsRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
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

    enum CullFaceType
    {
        t_back,             // 背面
        t_front,            // 正面
        t_frontAndBack      // 正面和背面
    };

    enum FrontFaceOrderType
    {
        t_CCW,      // 逆时针
        t_CW        // 顺时针
    };

    // 后期处理类型
    enum PostProcessType
    {
        t_Normal,
        t_Inversion,        // 反相
        t_GraysCale,        // 灰度
        t_Sharpen,          // 锐化
        t_Blur,             // 模糊
        t_EdgeDetection     // 边缘检测
    };

public:
    CubemapsRenderWidget(QWidget* parent = nullptr);
    ~CubemapsRenderWidget();

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

    // 设置/获取当前的后期处理效果
    void setCurrentPostProcessType(PostProcessType type);
    PostProcessType getCurrentPostProcessType(void);

    // ========================= 面剔除相关 ==================================================
public:
    // 设置面剔除相关
    void setCullFaceEnabled(bool isEnabled);
    bool isCullFaceEnabled(void);

    // 设置要剔除的面
    void setCullFaceType(CullFaceType type);
    CullFaceType getCullFaceType(void);

    // 设置正面的顺序
    void setFrontOrderType(FrontFaceOrderType type);
    FrontFaceOrderType getFrontOrderType(void);

private:
    bool m_isCullFaceEnabled = false;               // 默认关闭
    CullFaceType m_faceType = t_back;               // 默认为背面
    FrontFaceOrderType m_frontOrderType = t_CCW;    // 默认为逆时针 - 右手定则
    // =======================================================================================

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
    bool initCurrentFBOShaderProgram(void);

    GLuint m_shaderProgramId;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    QOpenGLShaderProgram* m_pLightShaderProgram = nullptr;
    QOpenGLShaderProgram* m_pGrassShaderProgram = nullptr;
    QOpenGLShaderProgram* m_pCurrentFBOShaderProgram = nullptr;
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
    COpenGLMesh* m_pCurrentFBOMesh = nullptr;
    COpenGLMesh* m_pCurrentFBOMesh2 = nullptr;

    // 灯
    void initLight(QOpenGLFunctions* f);
    void drawLight(void);

    // 盒子
    void initBox(QOpenGLFunctions* f);
    void initModelData(COpenGLMesh* pMesh);
    void drawBox(const QVector3D& pos, bool isInverted = false);
    void drawScaledBox(const QVector3D& pos);
    void drawTwoBox(void);
    void drawScaledTwoBox(void);
    // 绘制盒子的倒影
    void drawInvertedBox(void);

    // 地板
    void initFloor(QOpenGLFunctions* f);
    void initModelData2(void);      // 設置地板的数据
    void drawFloor(void);

    // 草
    void initGrass(QOpenGLFunctions* f);
    void initGrassModelData(COpenGLMesh* pMesh);
    void drawGrass(void);

    void initModelData3(void);

    // 天空盒
    void initSkyBox(QOpenGLFunctions* f);
    void drawSkyBox(void);
    COpenGLCubeTexture* m_pSkeyCubTexture = nullptr;
    COpenGLSkyBox* m_pSkyBox = nullptr;

    // 绘制整个场景
    void drawScene(void);

    // 绘制当前屏幕的FBO
    void initCurrentFBO(QOpenGLFunctions* f);
    void initCurrentFBO2(QOpenGLFunctions* f);
    void initCurrentFBOData(COpenGLMesh* pMesh);
    void initCurrentFBOData2(COpenGLMesh* pMesh);
    void drawCurrentFBO(void);
    void drawCurrentFBO2(void);

private:
    // FBO相关
    COpenGLFrameBufferObject* m_pFBO = nullptr;
    PostProcessType m_postProcessType = t_Normal;
    void processPostProcessType(PostProcessType type, QOpenGLShaderProgram* pShaderProgram);

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
