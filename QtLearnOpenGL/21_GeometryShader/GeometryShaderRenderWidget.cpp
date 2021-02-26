#include "GeometryShaderRenderWidget.h"
#include "OpenGLEngine/COpenGLTexture.h"
#include "OpenGLEngine/COpenGLRender.h"
#include "OpenGLEngine/COpenGLVertexObject.h"
#include "OpenGLEngine/CAttributePoint.h"
#include "OpenGLEngine/COpenGLCamera.h"
#include "OpenGLEngine/COpenGLMesh.h"
#include "Utils.h"
#include <QtMath>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

GeometryShaderRenderWidget::GeometryShaderRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
}

GeometryShaderRenderWidget::~GeometryShaderRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void GeometryShaderRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    initShaderProgram();
    initPoints();
}

void GeometryShaderRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void GeometryShaderRenderWidget::paintGL()
{
    m_pShaderProgram->bind();

    this->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    this->glDrawArrays(GL_POINTS, 0, 4);
    this->glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_pShaderProgram->release();
}

bool GeometryShaderRenderWidget::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/21_GeometryShader/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        return false;
    }

    // 添加几何着色器
    QString geometryShaderStr(":/21_GeometryShader/shader/gometryshader.gsh");
    QOpenGLShader* pGeometryShader = new QOpenGLShader(QOpenGLShader::Geometry, this);
    result = pGeometryShader->compileSourceFile(geometryShaderStr);
    if (!result)
    {
        qDebug() << pGeometryShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/21_GeometryShader/shader/fragmentshader.fsh");
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
    return m_pShaderProgram->link();
}

void GeometryShaderRenderWidget::initPoints(void)
{
    QVector<QVector3D> points;
    QVector3D pos1(-0.5f, 0.5f, 0.0f);
    QVector3D pos2(-0.5f, -0.5f, 0.0f);
    QVector3D pos3(0.5f, -0.5f, 0.0f);
    QVector3D pos4(0.5f, 0.5f, 0.0f);
    points << pos1 << pos2 << pos3 << pos4;

    QVector<QVector3D> colors;
    QVector3D color1(1.0f, 0.0f, 0.0f);
    QVector3D color2(0.0f, 1.0f, 0.0f);
    QVector3D color3(0.0f, 0.0f, 1.0f);
    QVector3D color4(1.5f, 1.0f, 0.0f);
    colors << color1 << color2 << color3 << color4;

    // 创建VBO
    this->glGenBuffers(1, &m_nVBO);
    this->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    int pointDataSize = points.size() * sizeof (QVector3D);
    int colorDataSize = colors.size() * sizeof (QVector3D);
    int nTotalSize = pointDataSize + colorDataSize;
    this->glBufferData(GL_ARRAY_BUFFER, nTotalSize, nullptr, GL_STATIC_DRAW);

    // 设置VBO的数据
    this->glBufferSubData(GL_ARRAY_BUFFER, 0, pointDataSize, points.data());
    this->glBufferSubData(GL_ARRAY_BUFFER, pointDataSize, colorDataSize, colors.data());

    // 设置 Attribute
    this->glEnableVertexAttribArray(0);
    this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
    this->glEnableVertexAttribArray(1);
    this->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)pointDataSize);

    this->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
