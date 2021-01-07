#ifndef TEXTURERENDERWIDGET_H
#define TEXTURERENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>

class TextureRenderWidget : public QOpenGLWidget, public QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    struct VertexAttributeData
    {
        // Postion
        float pos[3];
        float color[3];
        float coord[2];
    };

public:
    TextureRenderWidget(QWidget* parent = nullptr);
    ~TextureRenderWidget();

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
    void createVertexAttributeData(VertexAttributeData* pVetAttr);

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
    GLint m_nColorAttrLocationId;
    GLint m_nCoordAttrLocationId;

    // Uniform Location
    GLint m_nCoordLocationId;
    GLint m_nCoordLocationId2;
    GLint m_nMixNumLocationId;

    bool m_isFill = true;
    float m_mixNumber = 0.5f;
};
#endif
