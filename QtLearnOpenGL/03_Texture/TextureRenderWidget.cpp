#include "TextureRenderWidget.h"

#include <QDebug>

TextureRenderWidget::TextureRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{

}

TextureRenderWidget::~TextureRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void TextureRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化GPU程序
    bool result = initShaderProgram();
    if (!result)
        return;

    m_shaderProgramId = m_pShaderProgram->programId();
    // 获取位置和颜色的locationID
    m_nPosAttrLocationId = glGetAttribLocation(m_shaderProgramId, "pos");
    m_nColorAttrLocationId = glGetAttribLocation(m_shaderProgramId, "color");
    m_nCoordAttrLocationId = glGetAttribLocation(m_shaderProgramId, "texCoord");
    // 获取Uniform的Location
    m_nCoordLocationId = glGetUniformLocation(m_shaderProgramId, "texture1");
    m_nCoordLocationId2 = glGetUniformLocation(m_shaderProgramId, "texture2");
    m_nMixNumLocationId = glGetUniformLocation(m_shaderProgramId, "mixNumber");

    // 创建顶点属性数据
    VertexAttributeData vAttrData[4];
    createVertexAttributeData(vAttrData);
    // 创建IBO数据
    GLuint indices[] = {0, 1, 3, 1, 2, 3};

    // 创建VBO
    glGenBuffers(1, &m_nVBOId);
    // 创建IBO
    glGenBuffers(1, &m_nIBOId);

    // 初始化VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vAttrData), vAttrData, GL_STATIC_DRAW);

    // 初始化IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBOId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点信息属性指针
    glVertexAttribPointer(m_nPosAttrLocationId, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)0);
    glEnableVertexAttribArray(m_nPosAttrLocationId);
    // 设置原色信息属性指针
    glVertexAttribPointer(m_nColorAttrLocationId, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)(sizeof (float) * 3));
    glEnableVertexAttribArray(m_nColorAttrLocationId);
    // 设置纹理坐标
    glVertexAttribPointer(m_nCoordAttrLocationId, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)(sizeof (float) * 6));
    glEnableVertexAttribArray(m_nCoordAttrLocationId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 创建第一个纹理
    QImage image(":/03_Texture/image/TestImage1.jpg");
    image = image.convertToFormat(QImage::Format_RGB888);
    createTexture(image, m_nTextureId);

    // 创建第二个纹理
    QImage image2(":/03_Texture/image/TestImage2.jpg");
    image2 = image2.convertToFormat(QImage::Format_RGB888);
    createTexture(image2, m_nTextureId2);
}

void TextureRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    return QOpenGLWidget::resizeGL(w, h);
}

void TextureRenderWidget::paintGL()
{
    glClearColor(51.0f / 255.0f, 76.0f / 255.0f, 76.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    // 设置0号纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTextureId);
    glUniform1i(m_nCoordLocationId, 0);
    // 设置1号纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nTextureId2);
    glUniform1i(m_nCoordLocationId2, 1);
    // 设置混色的参数
    glUniform1f(m_nMixNumLocationId, m_mixNumber);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBOId);
    // 绘制
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    m_pShaderProgram->release();
}

bool TextureRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/03_Texture/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/03_Texture/shader/fragmentshader.fsh");
    m_pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = m_pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << m_pFragmentShader->log();
        return false;
    }

    // 创建ShaderProgram
    m_pShaderProgram = new QOpenGLShaderProgram(this);
    m_pShaderProgram->addShader(m_pVertexShader);
    m_pShaderProgram->addShader(m_pFragmentShader);
    return m_pShaderProgram->link();
}

void TextureRenderWidget::createVertexAttributeData(VertexAttributeData* pVetAttr)
{
    // 第一个点位置信息
    pVetAttr[0].pos[0] = -0.5f;
    pVetAttr[0].pos[1] = 0.5f;
    pVetAttr[0].pos[2] = 0.0f;
    // 第一个点颜色信息
    pVetAttr[0].color[0] = 1.0f;
    pVetAttr[0].color[1] = 0.0f;
    pVetAttr[0].color[2] = 0.0f;
    // 第一个点的纹理坐标
    pVetAttr[0].coord[0] = 0.0f;
    pVetAttr[0].coord[1] = 1.0f;

    // 第二个点位置信息
    pVetAttr[1].pos[0] = -0.5f;
    pVetAttr[1].pos[1] = -0.5f;
    pVetAttr[1].pos[2] = 0.0f;
    // 第二个点颜色信息
    pVetAttr[1].color[0] = 0.0f;
    pVetAttr[1].color[1] = 1.0f;
    pVetAttr[1].color[2] = 0.0f;
    // 第二个点的纹理坐标
    pVetAttr[1].coord[0] = 0.0f;
    pVetAttr[1].coord[1] = 0.0f;

    // 第三个点位置信息
    pVetAttr[2].pos[0] = 0.5f;
    pVetAttr[2].pos[1] = -0.5f;
    pVetAttr[2].pos[2] = 0.0f;
    // 第三个点颜色信息
    pVetAttr[2].color[0] = 0.0f;
    pVetAttr[2].color[1] = 0.0f;
    pVetAttr[2].color[2] = 1.0f;
    // 第三个点的纹理坐标
    pVetAttr[2].coord[0] = 1.0f;
    pVetAttr[2].coord[1] = 0.0f;

    // 第四个点位置信息
    pVetAttr[3].pos[0] = 0.5f;
    pVetAttr[3].pos[1] = 0.5f;
    pVetAttr[3].pos[2] = 0.0f;
    // 第四个点颜色信息
    pVetAttr[3].color[0] = 0.0f;
    pVetAttr[3].color[1] = 1.0f;
    pVetAttr[3].color[2] = 1.0f;
    // 第四个点的纹理坐标
    pVetAttr[3].coord[0] = 1.0f;
    pVetAttr[3].coord[1] = 1.0f;
}

void TextureRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool TextureRenderWidget::isFill(void)
{
    return m_isFill;
}

void TextureRenderWidget::setMixNumber(float number)
{
    m_mixNumber = number;
    this->update();
}

float TextureRenderWidget::getMixNumber(void)
{
    return m_mixNumber;
}

// 添加纹理
void TextureRenderWidget::createTexture(const QImage& image, GLuint& textureid)
{
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    // 设置纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, 0);
}
