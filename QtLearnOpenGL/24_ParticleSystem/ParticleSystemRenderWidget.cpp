#include "ParticleSystemRenderWidget.h"

ParticleSystemRenderWidget::ParticleSystemRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
    , m_myTestPos(0.0f, 0.0f, 0.0f)
{

}

ParticleSystemRenderWidget::~ParticleSystemRenderWidget()
{

}

bool ParticleSystemRenderWidget::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/24_ParticleSystem/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        return false;
    }

    // 加载几何着色器
    QString geometryShaderStr(":/24_ParticleSystem/shader/geometeryshader.gsh");
    QOpenGLShader* pGeometryShader = new QOpenGLShader(QOpenGLShader::Geometry, this);
    result = pGeometryShader->compileSourceFile(geometryShaderStr);
    if (!result)
    {
        qDebug() << pGeometryShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/24_ParticleSystem/shader/fragmentshader.fsh");
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

void ParticleSystemRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化shader
    initShaderProgram();

    // 创建2个Transform Feedback
    glGenTransformFeedbacks(2, m_translateFeedback);

#if 1
    glGenBuffers(2, m_vboBufferId);

    for (int i=0; i<2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_translateFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D), &m_myTestPos, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_translateFeedback[i]);
    }

    m_time.start();
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(1000 / 30);
    QObject::connect(m_pTimer, &QTimer::timeout, this, [&](void)->void{
        this->update();
    });
    m_pTimer->start();
#endif

//    m_pos << QVector3D(-0.5f, 0.5f, 0.0f) << QVector3D(-0.5f, -0.5f, 0.0f) << QVector3D(0.5f, -0.5f, 0.0f);
//    glGenBuffers(1, &m_nVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
//    glBufferData(GL_ARRAY_BUFFER, m_pos.size() * sizeof (QVector3D), m_pos.data(), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystemRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
}

void ParticleSystemRenderWidget::paintGL()
{
    float timeValue = m_time.elapsed();

    glClearColor(100.0 / 255, 100.0 / 255, 100.0/ 255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_pShaderProgram->bind();
    m_pShaderProgram->setUniformValue("renderTime", timeValue);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId[m_nCurrentTFIndex]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);

    glDrawArrays(GL_POINTS, 0, 1);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_pShaderProgram->release();
}

void ParticleSystemRenderWidget::updateData(void)
{
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId[m_nCurrentTFIndex]);
//    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_translateFeedback[m_nCurrentTFIndex]);
}
