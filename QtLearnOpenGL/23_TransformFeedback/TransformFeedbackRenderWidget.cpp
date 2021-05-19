#include "TransformFeedbackRenderWidget.h"
#include <QDebug>

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

    // 加载几何着色器
    QString geometryShaderStr(":/23_TransformFeedback/shader/geometeryshader.gsh");
    QOpenGLShader* pGeometryShader = new QOpenGLShader(QOpenGLShader::Geometry, this);
    result = pGeometryShader->compileSourceFile(geometryShaderStr);
    if (!result)
    {
        qDebug() << pGeometryShader->log();
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
    m_pShaderProgram->addShader(pGeometryShader);
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
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // 创建VBO
    GLfloat data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint attrInputId = glGetAttribLocation(m_pShaderProgram->programId(), "inValue");
    glEnableVertexAttribArray(attrInputId);
    glVertexAttribPointer(attrInputId, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

    glBindVertexArray(0);

    // 创建TBO
    glGenBuffers(1, &m_tbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (data) * 3, nullptr, GL_STATIC_READ);

    glBindVertexArray(0);

    // 创建查询
    glGenQueries(1, &m_queryId);
}

void TransformFeedbackRenderWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void TransformFeedbackRenderWidget::paintGL()
{
    static bool isFirstRender = true;
    if (isFirstRender)
    {
        isFirstRender = false;
    }
    else
        return;

    glClearColor(100.0 / 255, 100.0 / 255, 100.0/ 255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_pShaderProgram->bind();

    glBindVertexArray(m_vao);
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_tbo);

    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_queryId);
    glBeginTransformFeedback(GL_TRIANGLES);
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    glDisable(GL_RASTERIZER_DISCARD);

    glFlush();

    float result[15] = {0};
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(result), result);
    for (int i=0; i<15; ++i)
        qDebug() << result[i];
    qDebug() << "=====================================================";

    GLuint primitives;
    glGetQueryObjectuiv(m_queryId, GL_QUERY_RESULT, &primitives);
    qDebug() << primitives;

    m_pShaderProgram->release();
}
