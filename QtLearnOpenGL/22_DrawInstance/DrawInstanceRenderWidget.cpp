#include "DrawInstanceRenderWidget.h"
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

DrawInstaceRenderWidget::DrawInstaceRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
}

DrawInstaceRenderWidget::~DrawInstaceRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void DrawInstaceRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    initShaderProgram();
    initPoints();

    initMatrixs();
}

void DrawInstaceRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void DrawInstaceRenderWidget::paintGL()
{
    m_pShaderProgram->bind();

    this->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    this->glDrawArrays(GL_POINTS, 0, 4);
    this->glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_pShaderProgram->release();
}

bool DrawInstaceRenderWidget::initShaderProgram(void)
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

void DrawInstaceRenderWidget::initPoints(void)
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

void DrawInstaceRenderWidget::initMatrixs(void)
{
    m_matrixVecs.clear();
    int amount = m_nTotalSize;

    float radius = 50.0;
    float offset = 2.5f;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // 设置每一个陨石的模型矩阵
    for (int i=0; i<m_nTotalSize; ++i)
    {
        QMatrix4x4 model;
        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model.translate(QVector3D(x, y, z));

        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model.scale(scale);

        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = (rand() % 360);
        model.rotate(rotAngle, QVector3D(0.4f, 0.6f, 0.8f));

        // 4. 添加到矩阵的数组中
        m_matrixVecs << model;
    }

    // 创建VBO
    this->glGenBuffers(1, &m_nVBOId);
    this->glBindBuffer(GL_VERTEX_ARRAY, m_nVBOId);
    qDebug() << sizeof(QMatrix4x4);
    this->glBufferData(GL_VERTEX_ARRAY, m_matrixVecs.size() * sizeof (QMatrix4x4), m_matrixVecs.data(), GL_STATIC_DRAW);

    for (int i=0; i<m_nTotalSize; ++i)
    {
        // 创建VAO
        GLuint nVAOId;
        this->glGenBuffers(1, &nVAOId);
        this->glBindVertexArray(nVAOId);

        GLuint index = 0;
        GLuint p1 = index;
        GLuint p2 = p1 + 1;
        GLuint p3 = p2 + 1;
        GLuint p4 = p3 + 1;

        this->glEnableVertexAttribArray(p1);
        this->glVertexAttribPointer(p1, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)0);

        this->glEnableVertexAttribArray(p2);
        this->glVertexAttribPointer(p2, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(sizeof(QVector4D)));

        this->glEnableVertexAttribArray(p3);
        this->glVertexAttribPointer(p3, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(sizeof(QVector4D) * 2));

        this->glEnableVertexAttribArray(p4);
        this->glVertexAttribPointer(p4, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(sizeof(QVector4D) * 3));

        this->glVertexAttribDivisor(p1, 1);
        this->glVertexAttribDivisor(p2, 1);
        this->glVertexAttribDivisor(p3, 1);
        this->glVertexAttribDivisor(p4, 1);

        this->glBindVertexArray(0);

        m_VAOVecs << nVAOId;
    }
}
