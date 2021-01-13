#ifndef LIGHTBASERENDERWIDGET_H
#define LIGHTBASERENDERWIDGET_H

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
class COpenGLCamera;
class LightBaseRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
{
    Q_OBJECT

public:
    LightBaseRenderWidget(QWidget* parent = nullptr);
    ~LightBaseRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 混色参数
    void setMixNumber(float number);
    float getMixNumber(void);

    // 设置/获取相机的位置
    QVector3D getCameraPostion(void);
    void setCameraPostion(const QVector3D& vec);

    // 设置/获取相机的方向
    QVector3D getCameraFront(void);
    void setCameraFront(const QVector3D& vec);

    // 获取投影矩阵的角度
    float getProjMatrixAngle(void);
    void setProjMatrixAngle(float angle);

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

private:
    QVector3D m_cameraPos;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;

    float m_yaw = -90.0f;             // 偏航角，鼠标的X坐标的偏移量
    float m_pitch = 0.0f;           // 俯仰角，鼠标的Y坐标的偏移量

    float m_persAngle = 30.0f;      // 角度

    QPoint m_startPos;
    bool m_bPressed = false;

private:
    QColor m_cObjectColor;          // 物体的颜色
    QColor m_cLightColor;           // 光的颜色
    QVector3D m_lightPostion;       // 光源的位置

    COpenGLCamera* m_pCamera = nullptr;

signals:
    void cameraPostionChanged(const QVector3D& cameraPos);
    void cameraFrontChanged(const QVector3D& front);
    void projMatrixAngleChanged(float angle);
};
#endif
