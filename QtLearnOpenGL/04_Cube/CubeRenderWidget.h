#ifndef CUBERENDERWIDGET_H
#define CUBERENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QTimer>

class CubeRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    struct VertexAttributeData
    {
        // Postion
        float pos[3];
        float coord[2];
        float normal[3];
    };

    struct QuareData
    {
        VertexAttributeData p[4];
    };

public:
    CubeRenderWidget(QWidget* parent = nullptr);
    ~CubeRenderWidget();

    // 设置/获取线框模式
    void setFillStatus(bool isFill);
    bool isFill(void);

    // 混色参数
    void setMixNumber(float number);
    float getMixNumber(void);

    // 设置旋转方向
    void setRotationDirection(const QVector3D& vec);
    QVector3D getRotationDirection(void);

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

    GLuint m_nVBOId;
    GLuint m_nIBOId;

    GLuint m_nTextureId;
    GLuint m_nTextureId2;

    // Attribute Location
    GLint m_nPosAttrLocationId;
    GLint m_nCoordAttrLocationId;
    GLint m_nNormalAttrLocationId;

    // Uniform Location
    GLint m_nCoordLocationId;
    GLint m_nCoordLocationId2;
    GLint m_nMixNumLocationId;

    // MVP
    GLint m_nMLocationId;
    GLint m_nVLocationId;
    GLint m_nPLocationId;

    QMatrix4x4 m_MMat;
    QMatrix4x4 m_VMat;
    QMatrix4x4 m_PMat;

    bool m_isFill = true;
    float m_mixNumber = 0.5f;

private:
    QVector<QVector3D> m_points;                // 顶点数组
    QVector<QVector2D> m_textureCoords;         // 纹理坐标
    QVector<QVector3D> m_normalVNs;              // 法线
    QVector<VertexAttributeData> m_cube;

    // 初始化模型数据【立方体】
    void initModelData(void);

    // 旋转
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    void onTimeout(void);
    int m_angle = 0;
};
#endif
