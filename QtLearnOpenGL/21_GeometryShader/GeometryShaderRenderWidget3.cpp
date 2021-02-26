#include "GeometryShaderRenderWidget3.h"
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

GeometryShaderRenderWidget3::GeometryShaderRenderWidget3(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
}

GeometryShaderRenderWidget3::~GeometryShaderRenderWidget3()
{
    qDebug() << __FUNCTION__;
}

void GeometryShaderRenderWidget3::initializeGL()
{
    this->initializeOpenGLFunctions();

    initShaderProgram();
    initPoints();
    initModel();
    initCamera();
    initTimer();
}

void GeometryShaderRenderWidget3::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
    m_pCamera->setViewport(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void GeometryShaderRenderWidget3::paintGL()
{
    m_pShaderProgram->bind();

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置模型矩阵
    QMatrix4x4 modelMat;
    modelMat.translate(0.0f, -1.5f, -1.0f);
    modelMat.rotate(m_angle, QVector3D(0.0f, 1.0f, 0.0f));
    m_pShaderProgram->setUniformValue("M", modelMat);

    // 设置视口和投影矩阵
    m_pCamera->activeCamera(m_pShaderProgram);

    // 设置光的信息
    m_pShaderProgram->setUniformValue("lightMaterial.direction", QVector3D(1.0f, -0.5f, 1.5f));
    m_pShaderProgram->setUniformValue("lightMaterial.ambient", QVector3D(0.3f, 0.3f, 0.3f));
    m_pShaderProgram->setUniformValue("lightMaterial.diffuse", QVector3D(0.6f, 0.6f, 0.6f));
    m_pShaderProgram->setUniformValue("lightMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
    m_pShaderProgram->setUniformValue("objectMaterial.shininess", 32.0f);

    // 设置时间
    m_pShaderProgram->setUniformValue("time", (float)m_time.msec());

    // 绘制眼睛的位置
    m_pShaderProgram->setUniformValue("M_ViewPostion", m_pCamera->getCameraPostion());

    // 绘制模型
    m_pModel->draw();

    m_pShaderProgram->release();
}
void GeometryShaderRenderWidget3::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void GeometryShaderRenderWidget3::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void GeometryShaderRenderWidget3::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void GeometryShaderRenderWidget3::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void GeometryShaderRenderWidget3::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool GeometryShaderRenderWidget3::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/21_GeometryShader/shader/ModelVertexShader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        return false;
    }

    // 添加几何着色器
    QString geometryShaderStr(":/21_GeometryShader/shader/gometryshader3.gsh");
    QOpenGLShader* pGeometryShader = new QOpenGLShader(QOpenGLShader::Geometry, this);
    result = pGeometryShader->compileSourceFile(geometryShaderStr);
    if (!result)
    {
        qDebug() << pGeometryShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/21_GeometryShader/shader/ModelFragmentShader.fsh");
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

void GeometryShaderRenderWidget3::initModel(void)
{
    m_pModel = new COpenGLModel(QOpenGLContext::currentContext()->functions(), m_pShaderProgram, this);
    m_pModel->loadModel("./planet/planet.obj");
}

void GeometryShaderRenderWidget3::initCamera(void)
{
    // 设置相机
    if (!m_pCamera)
    {
        QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
        m_pCamera = new COpenGLCamera(f, this);
    }
    m_pCamera->setCameraShaderName("V", "P");
    m_pCamera->setCameraPostion(QVector3D(0.0f, -0.5f, 7.0f));
}

void GeometryShaderRenderWidget3::initPoints(void)
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

void GeometryShaderRenderWidget3::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(50);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &GeometryShaderRenderWidget3::onTimeout);
    m_pTimer->start();

    m_time = QTime::currentTime();
}

void GeometryShaderRenderWidget3::onTimeout(void)
{
    m_angle += 2.0;
    if (m_angle >= 360)
        m_angle -= 360;

    this->update();
}
