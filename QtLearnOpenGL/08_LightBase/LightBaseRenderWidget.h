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

    // 设置/获取物体的颜色
    void setObjectColor(const QColor& color);
    QColor getObjectColor(void);
    // 设置/获取光的颜色
    void setLightColor(const QColor& color);
    QColor getLightColor(void);
    // 设置/获取光的位置
    void setLightPostion(const QVector3D& pos);
    QVector3D getLightPostion(void);

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

private:
    QColor m_cObjectColor;          // 物体的颜色
    QColor m_cLightColor;           // 光的颜色
    QVector3D m_lightPostion;       // 光源的位置

    COpenGLCamera* m_pCamera = nullptr;
};
#endif
