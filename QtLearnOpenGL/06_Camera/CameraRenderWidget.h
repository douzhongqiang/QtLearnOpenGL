#ifndef CAMERARENDERWIDGET_H
#define CAMERARENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>

class COpenGLTexture;
class COpenGLRender;
class COpenGLVertexObject;
class CameraRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
{
    Q_OBJECT

public:
    CameraRenderWidget(QWidget* parent = nullptr);
    ~CameraRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 混色参数
    void setMixNumber(float number);
    float getMixNumber(void);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool initShaderProgram(void);
//    void createVertexAttributeData(VertexAttributeData* pVetAttr);

    // 添加纹理
    void createTexture(const QImage& image, GLuint& textureid);

    GLuint m_shaderProgramId;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
    QOpenGLShader* m_pVertexShader = nullptr;
    QOpenGLShader* m_pFragmentShader = nullptr;

    QMatrix4x4 m_MMat;
    QMatrix4x4 m_VMat;
    QMatrix4x4 m_PMat;

    bool m_isFill = true;
    float m_mixNumber = 0.5f;

private:
    QVector<QVector3D> m_points;                // 顶点数组
    QVector<QVector2D> m_textureCoords;         // 纹理坐标
    QVector<QVector3D> m_normalVNs;              // 法线

    QVector<QVector3D> m_transPos;

    // 初始化模型数据【立方体】
    void initModelData(void);

private:
    // 纹理
    COpenGLTexture* m_pTexture1 = nullptr;
    COpenGLTexture* m_pTexture2 = nullptr;
    // 节点对象
    COpenGLVertexObject* m_pVertexObject = nullptr;
    // 渲染对象
    COpenGLRender* m_pRender = nullptr;

    // 旋转
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    void onTimeout(void);
    float m_angle = 0.0f;
};
#endif
