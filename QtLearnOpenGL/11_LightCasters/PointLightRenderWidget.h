#ifndef POINTLIGHTRENDERWIDGET_H
#define POINTLIGHTRENDERWIDGET_H

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
class PointLightRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
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
        QVector3D lightPostion;       // 点光源的位置

        QVector3D ambientColor;         // 环境光颜色 - 物体的颜色
        QVector3D diffuesColor;         // 漫反射 - 物体的颜色
        QVector3D specularColor;        // 镜面反射颜色

        // 衰减相关
        float constant;     // 衰减常数
        float linear;       // 一次项系数
        float quadratic;    // 二次系数
    };

public:
    PointLightRenderWidget(QWidget* parent = nullptr);
    ~PointLightRenderWidget();

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

};

#endif
