#ifndef FLASHLIGHTRENDERWIDGET_H
#define FLASHLIGHTRENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>

// 点光源

class COpenGLTexture;
class COpenGLRender;
class COpenGLVertexObject;
class COpenGLCamera;
class FlashLightRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
{
    Q_OBJECT

public:
    struct ObjectMaterial
    {
//        QVector3D ambientColor;         // 环境光颜色 - 物体的颜色
//        QVector3D diffuesColor;         // 漫反射 - 物体的颜色
//        QVector3D specularColor;        // 镜面反射颜色

        float shininess = 32.0f;        // 界面反射系数
    };

    struct LightInfo
    {
        QVector3D lightPostion;       // 聚光灯的位置
        QVector3D direction;          // 聚光灯的方向
        float  cutout;                // 聚光灯的切角
        float  outerCutoff;

        QVector3D ambientColor;         // 环境光颜色 - 物体的颜色
        QVector3D diffuesColor;         // 漫反射 - 物体的颜色
        QVector3D specularColor;        // 镜面反射颜色
    };

public:
    FlashLightRenderWidget(QWidget* parent = nullptr);
    ~FlashLightRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 设置/获取物体的材质
    void setObjectMaterial(const ObjectMaterial& material);
    ObjectMaterial getObjectMaterial(void);

    // 设置/获取光的属性信息
    void setLightInfo(const LightInfo& info);
    LightInfo getLightInfo(void);

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
    QVector<QVector3D> m_points;                // 顶点数组
    QVector<QVector2D> m_textureCoords;         // 纹理坐标
    QVector<QVector3D> m_normalVNs;              // 法线

    QVector<QVector3D> m_transPos;

    // 初始化模型数据【立方体】
    void initModelData(void);

private:
    // 节点对象
    COpenGLVertexObject* m_pVertexObject = nullptr;
    // 渲染对象
    COpenGLRender* m_pRender = nullptr;

    // 物体的材质
    ObjectMaterial m_objectMaterial;
    void initObjectMaterial(void);

    // 光
    LightInfo m_light;
    void initLightInfo(void);

    // 漫反射贴图
    COpenGLTexture* m_diffuseTexture = nullptr;
    // 镜面反射贴图
    COpenGLTexture* m_specularTexture = nullptr;

private:
    COpenGLCamera* m_pCamera = nullptr;

private slots:
    void onCameraPostionChanged(const QVector3D& cameraPos);
    void onCameraFrontChanged(const QVector3D& front);

signals:
    void cameraInfoChanged(void);
};

#endif
