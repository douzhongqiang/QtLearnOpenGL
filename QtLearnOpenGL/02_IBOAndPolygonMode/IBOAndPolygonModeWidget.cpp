#include "IBOAndPolygonModeWidget.h"

#include <QDebug>

IBOAndPolygonModeWidget::IBOAndPolygonModeWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{

}

IBOAndPolygonModeWidget::~IBOAndPolygonModeWidget()
{

}

void IBOAndPolygonModeWidget::initializeGL()
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
    VertexAttributeData vAttrData[4];
    createVertexAttributeData(vAttrData);
    // 创建IBO数据
    GLuint indices[] = {0, 1, 3, 1, 2, 3};

    // 创建IBO
    glGenBuffers(1, &m_nIBOId);
    // 创建VBO
    glGenBuffers(1, &m_nVBOId);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IBOAndPolygonModeWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    return QOpenGLWidget::resizeGL(w, h);
}

void IBOAndPolygonModeWidget::paintGL()
{
    glClearColor(51.0f / 255.0f, 76.0f / 255.0f, 76.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    //glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
//    // 绘制
//    glDrawArrays(GL_POLYGON, 0, 4);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBOId);
    // 绘制
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    m_pShaderProgram->release();
}

bool IBOAndPolygonModeWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/02_IBOAndPolygonMode/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/02_IBOAndPolygonMode/shader/fragmentshader.fsh");
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

void IBOAndPolygonModeWidget::createVertexAttributeData(VertexAttributeData* pVetAttr)
{
    // 第一个点位置信息
    pVetAttr[0].pos[0] = -0.5f;
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

    // 第四个点位置信息
    pVetAttr[3].pos[0] = 0.5f;
    pVetAttr[3].pos[1] = 0.5f;
    pVetAttr[3].pos[2] = 0.0f;
    // 第四个点颜色信息
    pVetAttr[3].color[0] = 0.0f;
    pVetAttr[3].color[1] = 1.0f;
    pVetAttr[3].color[2] = 1.0f;
}

void IBOAndPolygonModeWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool IBOAndPolygonModeWidget::isFill(void)
{
    return m_isFill;
}
