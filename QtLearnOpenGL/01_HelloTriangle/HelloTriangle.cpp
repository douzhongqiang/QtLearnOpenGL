#include "HelloTriangle.h"
#include <QDebug>

HelloTriangleWidget::HelloTriangleWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{

}

HelloTriangleWidget::~HelloTriangleWidget()
{

}

void HelloTriangleWidget::initializeGL()
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

    // 创建顶点属性数据
    VertexAttributeData vAttrData[3];
    createVertexAttributeData(vAttrData);

    // 创建VBO
    glGenBuffers(1, &m_nVBOId);

    // 初始化VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vAttrData), vAttrData, GL_STATIC_DRAW);

    // 设置顶点信息属性指针
    glVertexAttribPointer(m_nPosAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)0);
    glEnableVertexAttribArray(m_nPosAttrLocationId);
    // 设置原色信息属性指针
    glVertexAttribPointer(m_nColorAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)(sizeof (float) * 3));
    glEnableVertexAttribArray(m_nColorAttrLocationId);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HelloTriangleWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    return QOpenGLWidget::resizeGL(w, h);
}

void HelloTriangleWidget::paintGL()
{
    glClearColor(51.0f / 255.0f, 76.0f / 255.0f, 76.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用shader
    m_pShaderProgram->bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
    // 绘制
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_pShaderProgram->release();
}

bool HelloTriangleWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/01_HelloTriangle/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/01_HelloTriangle/shader/fragmentshader.fsh");
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

void HelloTriangleWidget::createVertexAttributeData(VertexAttributeData* pVetAttr)
{
    // 第一个点位置信息
    pVetAttr[0].pos[0] = 0.0f;
    pVetAttr[0].pos[1] = 0.5f;
    pVetAttr[0].pos[2] = 0.0f;
    // 第一个点颜色信息
    pVetAttr[0].color[0] = 1.0f;
    pVetAttr[0].color[1] = 0.0f;
    pVetAttr[0].color[2] = 0.0f;

    // 第二个点位置信息
    pVetAttr[1].pos[0] = -0.5f;
    pVetAttr[1].pos[1] = -0.5f;
    pVetAttr[1].pos[2] = 0.0f;
    // 第二个点颜色信息
    pVetAttr[1].color[0] = 0.0f;
    pVetAttr[1].color[1] = 1.0f;
    pVetAttr[1].color[2] = 0.0f;

    // 第三个点位置信息
    pVetAttr[2].pos[0] = 0.5f;
    pVetAttr[2].pos[1] = -0.5f;
    pVetAttr[2].pos[2] = 0.0f;
    // 第三个点颜色信息
    pVetAttr[2].color[0] = 0.0f;
    pVetAttr[2].color[1] = 0.0f;
    pVetAttr[2].color[2] = 1.0f;
}
