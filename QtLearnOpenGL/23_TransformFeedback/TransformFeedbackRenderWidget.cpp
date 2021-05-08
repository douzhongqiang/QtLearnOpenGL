#include "TransformFeedbackRenderWidget.h"

TransformFeedbackRenderWidget::TransformFeedbackRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{

}

TransformFeedbackRenderWidget::~TransformFeedbackRenderWidget()
{

}

bool TransformFeedbackRenderWidget::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/23_TransformFeedback/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/23_TransformFeedback/shader/fragmentshader.fsh");
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << pFragmentShader->log();
        return false;
    }

    // 创建ShaderProgram
    m_pShaderProgram = new QOpenGLShaderProgram(this);
    m_pShaderProgram->addShader(pVertexShader);
    m_pShaderProgram->addShader(pFragmentShader);

    const GLchar* feedbackVarings[] = {"outValue"};
    this->glTransformFeedbackVaryings(m_pShaderProgram->programId(), 1, feedbackVarings, GL_INTERLEAVED_ATTRIBS);

    return m_pShaderProgram->link();
}

void TransformFeedbackRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    initShaderProgram();

    // 创建并绑定VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 创建VBO
    GLfloat data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint attrInputId = glGetAttribLocation(m_pShaderProgram->programId(), "inValue");
    glEnableVertexAttribArray(attrInputId);
    glVertexAttribPointer(attrInputId, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

    glBindVertexArray(0);

    // 创建TBO
}

void TransformFeedbackRenderWidget::resizeGL(int w, int h)
{
    glClearColor(100.0 / 255, 100.0 / 255, 100.0/ 255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void TransformFeedbackRenderWidget::paintGL()
{

}
