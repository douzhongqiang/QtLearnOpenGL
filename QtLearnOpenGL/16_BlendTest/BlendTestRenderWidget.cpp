#include "BlendTestRenderWidget.h"
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

BlendTestRenderWidget::BlendTestRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    initLightInfo();
}

BlendTestRenderWidget::~BlendTestRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void BlendTestRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化GPU程序
    initShaderProgram();
    initLightShaderProgram();
    initGrassShaderProgram();

    glEnable(GL_DEPTH_TEST);

    m_shaderProgramId = m_pShaderProgram->programId();

    // 设置纹理等
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

    // 设置相机
    if (!m_pCamera)
        m_pCamera = new COpenGLCamera(f, this);
    m_pCamera->setCameraShaderName("V", "P");
    m_pCamera->setCameraPostion(QVector3D(0.0f, -1.0f, 7.0f));
    QObject::connect(m_pCamera, &COpenGLCamera::cameraPostionChanged, this, &BlendTestRenderWidget::attributeInfoChanged);
    QObject::connect(m_pCamera, &COpenGLCamera::cameraFrontChanged, this, &BlendTestRenderWidget::attributeInfoChanged);

    initLight(f);
    initBox(f);
    initFloor(f);
    initGrass(f);

    initTimer();
}

void BlendTestRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
    m_pCamera->setViewport(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void BlendTestRenderWidget::paintGL()
{
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 绘制灯光
    m_pCamera->activeCamera(m_pLightShaderProgram);
    drawLight();

    // 使用shader
    m_pShaderProgram->bind();

    // 设置当前显示的内容
    m_pShaderProgram->setUniformValue("M_isShowDepthTest", m_bShowDepthTest);

    // 设置光的信息
    m_pShaderProgram->setUniformValue("lightMaterial.direction", m_light.lightPos);
    m_pShaderProgram->setUniformValue("lightMaterial.ambient", m_light.ambientColor);
    m_pShaderProgram->setUniformValue("lightMaterial.diffuse", m_light.diffuesColor);
    m_pShaderProgram->setUniformValue("lightMaterial.specular", m_light.specularColor);
    m_pShaderProgram->setUniformValue("objectMaterial.shininess", 32.0f);

    // 設置眼睛的位置
    if (m_pCamera)
    {
        QVector3D cameraPos = m_pCamera->getCameraPostion();
        m_pShaderProgram->setUniformValue("M_ViewPostion", cameraPos);
    }

    // 设置视图矩阵和投影矩阵
    m_pCamera->activeCamera(m_pShaderProgram);

    // 绘制地板
    drawFloor();

    // 绘制盒子
    drawTwoBox();
    m_pShaderProgram->release();

    // 绘制小草
    m_pCamera->activeCamera(m_pGrassShaderProgram);
    drawGrass();
}

void BlendTestRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void BlendTestRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void BlendTestRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void BlendTestRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void BlendTestRenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool BlendTestRenderWidget::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/14_DepthTest/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/14_DepthTest/shader/fragmentshader.fsh");
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

bool BlendTestRenderWidget::initLightShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/14_DepthTest/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        pVertexShader->deleteLater();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/15_StencilTest/shader/lightfragmentShader.fsh");
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << m_pFragmentShader->log();
        pFragmentShader->deleteLater();
        pVertexShader->deleteLater();
        return false;
    }

    // 创建ShaderProgram
    m_pLightShaderProgram = new QOpenGLShaderProgram(this);
    m_pLightShaderProgram->addShader(pVertexShader);
    m_pLightShaderProgram->addShader(pFragmentShader);
    return m_pLightShaderProgram->link();
}

bool BlendTestRenderWidget::initGrassShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/14_DepthTest/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        pVertexShader->deleteLater();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/16_BlendTest/shader/blendFragmentShader.fsh");
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << m_pFragmentShader->log();
        pFragmentShader->deleteLater();
        pVertexShader->deleteLater();
        return false;
    }

    // 创建ShaderProgram
    m_pGrassShaderProgram = new QOpenGLShaderProgram(this);
    m_pGrassShaderProgram->addShader(pVertexShader);
    m_pGrassShaderProgram->addShader(pFragmentShader);
    return m_pGrassShaderProgram->link();
}

void BlendTestRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool BlendTestRenderWidget::isFill(void)
{
    return m_isFill;
}

// 设置/获取光的属性信息
void BlendTestRenderWidget::setLightInfo(const LightInfo& info)
{
    m_light = info;
    this->update();
}

BlendTestRenderWidget::LightInfo BlendTestRenderWidget::getLightInfo(void)
{
    return m_light;
}

void BlendTestRenderWidget::setCameraPostion(const QVector3D& postion)
{
    m_pCamera->setCameraPostion(postion);
}

QVector3D BlendTestRenderWidget::getCameraPostion(void)
{
    return m_pCamera->getCameraPostion();
}

void BlendTestRenderWidget::setCameraFront(const QVector3D& front)
{
    m_pCamera->setCameraFront(front);
}

QVector3D BlendTestRenderWidget::getCameraFront(void)
{
    return m_pCamera->getCameraFront();
}

// 设置是否显示为深度测试结果
void BlendTestRenderWidget::setDepthTestVisible(bool isVisible)
{
    m_bShowDepthTest = isVisible;
    this->update();
}

bool BlendTestRenderWidget::isDepthTestVisible(void)
{
    return m_bShowDepthTest;
}

void BlendTestRenderWidget::initLightInfo(void)
{
    m_light.lightPos = QVector3D(1.0f, -0.5f, 1.5f);

    m_light.ambientColor = QVector3D(0.3f, 0.3f, 0.3f);
    m_light.diffuesColor = QVector3D(0.6f, 0.6f, 0.6f);
    m_light.specularColor = QVector3D(1.0f, 1.0f, 1.0f);

    this->update();
}

void BlendTestRenderWidget::initLight(QOpenGLFunctions* f)
{
    m_pLightMesh = new COpenGLMesh(f, m_pLightShaderProgram, this);
    initModelData(m_pLightMesh);
}

void BlendTestRenderWidget::drawLight(void)
{
    m_pLightShaderProgram->bind();

    // 设置模型矩阵
    QMatrix4x4 mat;
    mat.translate(m_light.lightPos);
    mat.scale(0.1f);
    m_pLightShaderProgram->setUniformValue("M", mat);
    m_pLightShaderProgram->setUniformValue("M_Color", QVector3D(1.0f, 1.0f, 1.0f));

    // 设置V和P Matrix
    m_pLightShaderProgram->setUniformValue("V", m_pCamera->getVMatrix());
    m_pLightShaderProgram->setUniformValue("P", m_pCamera->getPMatrix());

    // 绘制网格
    m_pLightMesh->draw();

    m_pLightShaderProgram->release();
}

void BlendTestRenderWidget::initBox(QOpenGLFunctions* f)
{
    m_pMesh = new COpenGLMesh(f, m_pShaderProgram, this);
    m_pTwoBoxMesh = new COpenGLMesh(f, m_pLightShaderProgram, this);

    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->create();
    pMeshTexture->setImage(":/14_DepthTest/image/marble.jpg");
    pMeshTexture->setType(COpenGLTexture::t_diffuse);
    m_pMesh->addTexture(pMeshTexture);

    COpenGLTexture* pMeshTexture2 = new COpenGLTexture(f, this);
    pMeshTexture2->create();
    pMeshTexture2->setImage(":/10_LightingMaps/image/container2_specular.png");
    pMeshTexture2->setType(COpenGLTexture::t_specular);
    m_pMesh->addTexture(pMeshTexture2);

    initModelData(m_pMesh);
    initModelData(m_pTwoBoxMesh);
}

void BlendTestRenderWidget::initModelData(COpenGLMesh* pMesh)
{
    CAttributePointArray arrays;

    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

    int interval = 8;
    int pointSize = sizeof(vertices) / sizeof(float) / interval;
    QVector<unsigned int> vecs;

    for (int i=0; i<pointSize; ++i)
    {
        int index = i * interval;

        CAttributePoint point;
        point.pos[0] = vertices[index];
        point.pos[1] = vertices[index + 1];
        point.pos[2] = vertices[index + 2];

        point.normal[0] = vertices[index + 3];
        point.normal[1] = vertices[index + 4];
        point.normal[2] = vertices[index + 5];

        point.textureCoord[0] = vertices[index + 6];
        point.textureCoord[1] = vertices[index + 7];

        arrays.append(point);

        vecs << i;
    }
    pMesh->setPoints(arrays);
    pMesh->setIndices(vecs);

    pMesh->setupMesh();
}

void BlendTestRenderWidget::drawBox(const QVector3D& pos)
{
    QMatrix4x4 mat;
    mat.translate(pos);
    m_pShaderProgram->setUniformValue("M", mat);

    m_pMesh->draw();
}

void BlendTestRenderWidget::drawScaledBox(const QVector3D& pos)
{
    QMatrix4x4 mat;
    mat.translate(pos);
    mat.scale(1.015f);
    m_pLightShaderProgram->setUniformValue("M", mat);
    m_pLightShaderProgram->setUniformValue("M_Color", QVector3D(0.78, 0.48, 0.17));

    m_pTwoBoxMesh->draw();
}

void BlendTestRenderWidget::drawTwoBox(void)
{
    // 绘制盒子
    drawBox(QVector3D(0.0f, -1.5f, 0.0f));
    drawBox(QVector3D(2.0f, -1.5f, 2.0f));
}

void BlendTestRenderWidget::drawScaledTwoBox(void)
{
    m_pLightShaderProgram->bind();

    // 绘制盒子
    drawScaledBox(QVector3D(0.0f, -1.5f, 0.0f));
    drawScaledBox(QVector3D(2.0f, -1.5f, 2.0f));

    m_pLightShaderProgram->release();
}

void BlendTestRenderWidget::initModelData2(void)
{
    CAttributePointArray arrays;

    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

             0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
        };

//    float vertices[] = {
//            // positions          // normals           // texture coords
//            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
//             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

//            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
//             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

//            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

//             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

//            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
//             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

//            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
//             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
//        };


    int interval = 8;
    int pointSize = sizeof(vertices) / sizeof(float) / interval;
    QVector<unsigned int> vecs;

    for (int i=0; i<pointSize; ++i)
    {
        int index = i * interval;

        CAttributePoint point;
        point.pos[0] = vertices[index];
        point.pos[1] = vertices[index + 1];
        point.pos[2] = vertices[index + 2];

        point.normal[0] = vertices[index + 3];
        point.normal[1] = vertices[index + 4];
        point.normal[2] = vertices[index + 5];

        point.textureCoord[0] = vertices[index + 6];
        point.textureCoord[1] = vertices[index + 7];

        arrays.append(point);

        vecs << i;
    }
    m_pMeshFloor->setPoints(arrays);
    m_pMeshFloor->setIndices(vecs);

    m_pMeshFloor->setupMesh();
}

void BlendTestRenderWidget::initModelData3(void)
{
    QVector<QVector3D> m_points;                // 顶点数组
    QVector<QVector2D> m_textureCoords;         // 纹理坐标
    QVector<QVector3D> m_normalVNs;              // 法线

    m_points.clear();
    m_textureCoords.clear();
    m_normalVNs.clear();

    // 顶点数据
    m_points.push_back(QVector3D(1.000000f, 1.000000f, -1.000000f));
    m_points.push_back(QVector3D(1.000000f, -1.000000f, -1.000000f));
    m_points.push_back(QVector3D(1.000000f, 1.000000f, 1.000000f));
    m_points.push_back(QVector3D(1.000000f, -1.000000f, 1.000000f));
    m_points.push_back(QVector3D(-1.000000f, 1.000000f, -1.000000f));
    m_points.push_back(QVector3D(-1.000000f, -1.000000f, -1.000000f));
    m_points.push_back(QVector3D(-1.000000f, 1.000000f, 1.000000f));
    m_points.push_back(QVector3D(-1.000000f, -1.000000f, 1.000000f));

    // 纹理坐标
    m_textureCoords.push_back(QVector2D(0.625000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.875000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.875000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 1.000000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 1.000000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.000000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.000000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.250000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.250000f));
    m_textureCoords.push_back(QVector2D(0.125000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.125000f, 0.750000f));

    // 法线
    m_normalVNs.push_back(QVector3D(0.0000f, 1.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, 0.0000f, 1.0000f));
    m_normalVNs.push_back(QVector3D(-1.0000f, 0.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, -1.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(1.0000f, 0.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, 0.0000f, -1.0000f));

    int facePoint[] = {1, 5, 7, 3, \
                       4, 3, 7, 8, \
                       8, 7, 5, 6, \
                       6, 2, 4, 8, \
                       2, 1, 3, 4, \
                       6, 5, 1, 2};

    int faceCoord[] = {1, 2, 3, 4, \
                       5, 4, 6, 7, \
                       8, 9, 10, 11, \
                       12, 13, 5, 14, \
                       13, 1, 4, 5, \
                       11, 10, 1, 13};

    int faceNoemal[] = {1, 1, 1, 1, \
                        2, 2, 2, 2, \
                        3, 3, 3, 3, \
                        4, 4, 4, 4, \
                        5, 5, 5, 5, \
                        6, 6, 6, 6};

    CAttributePointArray arrays;
    QVector<unsigned int> vecs;

    int count = sizeof(facePoint) / sizeof(int);
    for (int i=0; i<count; ++i)
    {
        CAttributePoint vertexAttr;

        // 点坐标
        int index = facePoint[i] - 1;
        vertexAttr.pos[0] = m_points[index].x();
        vertexAttr.pos[1] = m_points[index].y();
        vertexAttr.pos[2] = m_points[index].z();

        // 纹理坐标
        index = faceCoord[i] - 1;
        vertexAttr.textureCoord[0] = m_textureCoords[index].x();
        vertexAttr.textureCoord[1] = m_textureCoords[index].y();

        // 法线
        index = faceNoemal[i] - 1;
        vertexAttr.normal[0] = m_normalVNs[index].x();
        vertexAttr.normal[1] = m_normalVNs[index].y();
        vertexAttr.normal[2] = m_normalVNs[index].z();

        arrays.push_back(vertexAttr);
        vecs << i;
    }

    m_pMeshFloor->setPoints(arrays);
//    m_pMeshFloor->setIndices(vecs);
    m_pMeshFloor->setObjectType(COpenGLVertexObject::t_Quads);

    m_pMeshFloor->setupMesh();
}

void BlendTestRenderWidget::initFloor(QOpenGLFunctions* f)
{
    m_pMeshFloor = new COpenGLMesh(f, m_pShaderProgram, this);

    // 添加地板紋理
    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->create();
    pMeshTexture->setImage(":/14_DepthTest/image/metal.png");
    pMeshTexture->setType(COpenGLTexture::t_diffuse);
    m_pMeshFloor->addTexture(pMeshTexture);

    // 設置地板的顶点数据
    initModelData2();
}

void BlendTestRenderWidget::drawFloor(void)
{
    QMatrix4x4 mat;
    mat.translate(QVector3D(0.0f, -2.0f, 0.0f));
    mat.scale(15.0f, 15.0f, 15.0f);
    m_pShaderProgram->setUniformValue("M", mat);

    m_pMeshFloor->draw();
}

// 草
void BlendTestRenderWidget::initGrass(QOpenGLFunctions* f)
{
    m_pGrassMesh = new COpenGLMesh(f, m_pGrassShaderProgram, this);

    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->setFilterType(COpenGLTexture::t_nearest);
    pMeshTexture->create();
//    pMeshTexture->setImage(":/16_BlendTest/image/grass.png");
    pMeshTexture->setImage(":/16_BlendTest/image/blending_transparent_window.png");
    pMeshTexture->setType(COpenGLTexture::t_diffuse);
    m_pGrassMesh->addTexture(pMeshTexture);

    initGrassModelData(m_pGrassMesh);

    m_grassPosVec << QVector3D(2.5f,  -1.5f, -2.6f);
    m_grassPosVec << QVector3D(-1.3f,  -1.5f, -2.3f);
    m_grassPosVec << QVector3D(0.5f, -1.5f, 1.0f);
    m_grassPosVec << QVector3D(0.0f,  -1.5f,  2.7f);
    m_grassPosVec << QVector3D(1.5f,  -1.5f,  3.0f);
}

void BlendTestRenderWidget::initGrassModelData(COpenGLMesh* pMesh)
{
    CAttributePointArray arrays;

    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f
        };

    int interval = 8;
    int pointSize = sizeof(vertices) / sizeof(float) / interval;
    QVector<unsigned int> vecs;

    for (int i=0; i<pointSize; ++i)
    {
        int index = i * interval;

        CAttributePoint point;
        point.pos[0] = vertices[index];
        point.pos[1] = vertices[index + 1];
        point.pos[2] = vertices[index + 2];

        point.normal[0] = vertices[index + 3];
        point.normal[1] = vertices[index + 4];
        point.normal[2] = vertices[index + 5];

        point.textureCoord[0] = vertices[index + 6];
        point.textureCoord[1] = vertices[index + 7];

        arrays.append(point);

        vecs << i;
    }
    pMesh->setPoints(arrays);
    pMesh->setIndices(vecs);

    pMesh->setupMesh();
}

void BlendTestRenderWidget::drawGrass(void)
{
    m_pGrassShaderProgram->bind();

    // 设置V和P Matrix
    m_pGrassShaderProgram->setUniformValue("V", m_pCamera->getVMatrix());
    m_pGrassShaderProgram->setUniformValue("P", m_pCamera->getPMatrix());

    // 需要根据距离眼睛位置的距离排序
    for (auto iter = m_grassPosVec.begin(); iter != m_grassPosVec.end(); ++iter)
    {
        // 设置草的位置
        QMatrix4x4 mat;
        mat.translate(*iter);
        m_pGrassShaderProgram->setUniformValue("M", mat);

        // 绘制草
        m_pGrassMesh->draw();
    }

    m_pGrassShaderProgram->release();
}

void BlendTestRenderWidget::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(50);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &BlendTestRenderWidget::onTimeout);
    m_pTimer->start();
}

void BlendTestRenderWidget::onTimeout(void)
{
    m_angle += 2.0;
//    qDebug() << __FUNCTION__ << m_angle;

    this->update();
}
