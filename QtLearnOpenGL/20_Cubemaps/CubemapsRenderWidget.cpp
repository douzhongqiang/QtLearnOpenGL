#include "CubemapsRenderWidget.h"
#include "OpenGLEngine/COpenGLTexture.h"
#include "OpenGLEngine/COpenGLRender.h"
#include "OpenGLEngine/COpenGLVertexObject.h"
#include "OpenGLEngine/CAttributePoint.h"
#include "OpenGLEngine/COpenGLCamera.h"
#include "OpenGLEngine/COpenGLMesh.h"
#include "COpenGLCubeTexture.h"
#include "COpenGLSkyBox.h"
#include "Utils.h"
#include <QtMath>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

CubemapsRenderWidget::CubemapsRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    initLightInfo();
}

CubemapsRenderWidget::~CubemapsRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void CubemapsRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化GPU程序
    initShaderProgram();
    initLightShaderProgram();
    initGrassShaderProgram();
    initCurrentFBOShaderProgram();
    m_pReflectionAndRefractionShaderProgram = this->createShaderProgram(":/20_Cubemaps/shader/ReflectionAndRefractionVertexShader.vsh", \
                                                                        ":/20_Cubemaps/shader/ReflectionAndRefractionFragmentShader.fsh");

    glEnable(GL_DEPTH_TEST);

    m_shaderProgramId = m_pShaderProgram->programId();

    // 设置纹理等
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

    // 设置相机
    if (!m_pCamera)
        m_pCamera = new COpenGLCamera(f, this);
    m_pCamera->setCameraShaderName("V", "P");
    m_pCamera->setCameraPostion(QVector3D(0.0f, -0.5f, 7.0f));
    QObject::connect(m_pCamera, &COpenGLCamera::cameraPostionChanged, this, &CubemapsRenderWidget::attributeInfoChanged);
    QObject::connect(m_pCamera, &COpenGLCamera::cameraFrontChanged, this, &CubemapsRenderWidget::attributeInfoChanged);

    initLight(f);
    initBox(f);
    initFloor(f);
    initGrass(f);
    initCurrentFBO(f);
    initCurrentFBO2(f);
    initSkyBox(f);
    initReflectionBox(f);
    initRefractionBox(f);

    // 初始化FBO
    int width = this->width();
    int height = this->height();
    qDebug() << width << height;
    m_pFBO = new COpenGLFrameBufferObject(width, height, this);
    m_pFBO->create();

    initTimer();
}

void CubemapsRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
    m_pCamera->setViewport(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void CubemapsRenderWidget::paintGL()
{
    // 绘制当前FBO
    m_pCurrentFBOShaderProgram->bind();

    processPostProcessType(m_postProcessType, m_pCurrentFBOShaderProgram);
    drawCurrentFBO();
//    drawCurrentFBO2();

    m_pCurrentFBOShaderProgram->release();
}

void CubemapsRenderWidget::processPostProcessType(PostProcessType type, QOpenGLShaderProgram* pShaderProgram)
{
    if ((int)type <= (int)t_GraysCale)
        pShaderProgram->setUniformValue("M_Type", (int)type);
    else
        pShaderProgram->setUniformValue("M_Type", 3);

    switch (type)
    {
    case t_Sharpen:
    {
        float matValue[] = {2.0f, 2.0f, 2.0f, \
                            2.0f, -15.0f, 2.0f, \
                            2.0f, 2.0f, 2.0f};
        QMatrix3x3 kernelMat(matValue);
        m_pCurrentFBOShaderProgram->setUniformValue("M_kernel", kernelMat);
        break;
    }
    case t_Blur:
    {
        float matValue[] = {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f, \
                            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f, \
                            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f};
        QMatrix3x3 kernelMat(matValue);
        m_pCurrentFBOShaderProgram->setUniformValue("M_kernel", kernelMat);
        break;
    }
    case t_EdgeDetection:
    {
        float matValue[] = {1.0f, 1.0f, 1.0f, \
                            1.0f, -8.0f, 1.0f, \
                            1.0f, 1.0f, 1.0f};
        QMatrix3x3 kernelMat(matValue);
        m_pCurrentFBOShaderProgram->setUniformValue("M_kernel", kernelMat);
        break;
    }
    default:
        break;
    }
}

// 绘制整个场景
void CubemapsRenderWidget::drawScene(void)
{
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 开启蒙版测试
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    glClearColor(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置相机矩阵的值
    m_pCamera->activeCamera(m_pLightShaderProgram);

    // 绘制天空盒
    glStencilMask(0x00);
    drawSkyBox();

    // 绘制点光源
    drawLight();

    // 绘制反射Box
    drawReflectionBox();

    // 使用shader
    m_pShaderProgram->bind();

    // 设置光的信息
    m_pShaderProgram->setUniformValue("lightMaterial.enabled", true);
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

    glStencilMask(0x00);
    // 绘制盒子
    drawTwoBox();

    // 写入蒙版缓冲区
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    // 绘制地板
    glDepthMask(GL_FALSE);
    drawFloor();
    glDepthMask(GL_TRUE);

    // 绘制盒子的倒影
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    drawInvertedBox();
    glStencilMask(0xFF);
    m_pShaderProgram->release();
}

// 绘制当前屏幕的FBO
void CubemapsRenderWidget::drawCurrentFBO(void)
{
    glClearColor(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 关闭深度测试和蒙版测试
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    // 设置纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_pFBO->getTextureId());
    m_pCurrentFBOShaderProgram->setUniformValue("fboImage", 0);

    // 绘制
    m_pCurrentFBOMesh->draw();
}

void CubemapsRenderWidget::drawCurrentFBO2(void)
{
//    glClearColor(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
//    glClearStencil(0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 关闭深度测试和蒙版测试
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    // 设置纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_pFBO->getTextureId());
    m_pCurrentFBOShaderProgram->setUniformValue("fboImage", 0);

    // 绘制
    m_pCurrentFBOMesh2->draw();
}

void CubemapsRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void CubemapsRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void CubemapsRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void CubemapsRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void CubemapsRenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool CubemapsRenderWidget::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/17_InvertedImage/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/17_InvertedImage/shader/fragmentshader.fsh");
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

bool CubemapsRenderWidget::initLightShaderProgram(void)
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

bool CubemapsRenderWidget::initGrassShaderProgram(void)
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

bool CubemapsRenderWidget::initCurrentFBOShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/19_FrameBuffer/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        pVertexShader->deleteLater();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/19_FrameBuffer/shader/fragmentshader.fsh");
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
    m_pCurrentFBOShaderProgram = new QOpenGLShaderProgram(this);
    m_pCurrentFBOShaderProgram->addShader(pVertexShader);
    m_pCurrentFBOShaderProgram->addShader(pFragmentShader);
    return m_pCurrentFBOShaderProgram->link();
}

QOpenGLShaderProgram* CubemapsRenderWidget::createShaderProgram(const QString& vertexShader, const QString& fragmentShader)
{
    // 加载顶点着色器
    QString vertexShaderStr(vertexShader);
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        pVertexShader->deleteLater();
        return nullptr;
    }

    // 加载片段着色器
    QString fragmentShaderStr(fragmentShader);
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << pFragmentShader->log();
        pFragmentShader->deleteLater();
        pVertexShader->deleteLater();
        return nullptr;
    }

    // 创建ShaderProgram
    QOpenGLShaderProgram* pShaderProgram = new QOpenGLShaderProgram(this);
    pShaderProgram->addShader(pVertexShader);
    pShaderProgram->addShader(pFragmentShader);
    pShaderProgram->link();

    return pShaderProgram;
}

void CubemapsRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool CubemapsRenderWidget::isFill(void)
{
    return m_isFill;
}

// 设置/获取光的属性信息
void CubemapsRenderWidget::setLightInfo(const LightInfo& info)
{
    m_light = info;
    this->update();
}

CubemapsRenderWidget::LightInfo CubemapsRenderWidget::getLightInfo(void)
{
    return m_light;
}

void CubemapsRenderWidget::setCameraPostion(const QVector3D& postion)
{
    m_pCamera->setCameraPostion(postion);
}

QVector3D CubemapsRenderWidget::getCameraPostion(void)
{
    return m_pCamera->getCameraPostion();
}

void CubemapsRenderWidget::setCameraFront(const QVector3D& front)
{
    m_pCamera->setCameraFront(front);
}

QVector3D CubemapsRenderWidget::getCameraFront(void)
{
    return m_pCamera->getCameraFront();
}

// 设置是否显示为深度测试结果
void CubemapsRenderWidget::setDepthTestVisible(bool isVisible)
{
    m_bShowDepthTest = isVisible;
    this->update();
}

bool CubemapsRenderWidget::isDepthTestVisible(void)
{
    return m_bShowDepthTest;
}

// 设置/获取当前的后期处理效果
void CubemapsRenderWidget::setCurrentPostProcessType(PostProcessType type)
{
    m_postProcessType = type;
    this->update();
}

CubemapsRenderWidget::PostProcessType CubemapsRenderWidget::getCurrentPostProcessType(void)
{
    return m_postProcessType;
}

void CubemapsRenderWidget::initLightInfo(void)
{
    m_light.lightPos = QVector3D(1.0f, -0.5f, 1.5f);

    m_light.ambientColor = QVector3D(0.3f, 0.3f, 0.3f);
    m_light.diffuesColor = QVector3D(0.6f, 0.6f, 0.6f);
    m_light.specularColor = QVector3D(1.0f, 1.0f, 1.0f);

    this->update();
}

void CubemapsRenderWidget::initLight(QOpenGLFunctions* f)
{
    m_pLightMesh = new COpenGLMesh(f, m_pLightShaderProgram, this);
    initModelData(m_pLightMesh);
}

void CubemapsRenderWidget::drawLight(void)
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

void CubemapsRenderWidget::initBox(QOpenGLFunctions* f)
{
    m_pMesh = new COpenGLMesh(f, m_pShaderProgram, this);
    m_pTwoBoxMesh = new COpenGLMesh(f, m_pLightShaderProgram, this);

    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->create();
//    pMeshTexture->setImage("TestTexture.jpg");
    pMeshTexture->setImage(":/10_LightingMaps/image/uqahruewcx.jpeg");
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

void CubemapsRenderWidget::initModelData(COpenGLMesh* pMesh)
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

void CubemapsRenderWidget::drawBox(const QVector3D& pos, bool isInverted)
{
    QMatrix4x4 mat;
    mat.translate(pos);
    mat.rotate(m_angle, QVector3D(0.0f, 1.0f, 0.0f));
    if (isInverted)
        mat.scale(1.0f, -1.0f, 1.0f);
    m_pShaderProgram->setUniformValue("M", mat);

    m_pMesh->draw();
}

void CubemapsRenderWidget::drawScaledBox(const QVector3D& pos)
{
    QMatrix4x4 mat;
    mat.translate(pos);
    mat.scale(1.015f);
    m_pLightShaderProgram->setUniformValue("M", mat);
    m_pLightShaderProgram->setUniformValue("M_Color", QVector3D(0.78, 0.48, 0.17));

    m_pTwoBoxMesh->draw();
}

void CubemapsRenderWidget::drawTwoBox(void)
{
    m_pShaderProgram->setUniformValue("objectMaterial.usedTexture", true);
    m_pShaderProgram->setUniformValue("objectFactor", QVector3D(1.0f, 1.0f, 1.0f));

    // 绘制盒子
    drawBox(QVector3D(0.0f, -1.5f, 0.0f));
    drawBox(QVector3D(2.0f, -1.5f, 2.0f));
}

void CubemapsRenderWidget::drawInvertedBox(void)
{
    m_pShaderProgram->setUniformValue("lightMaterial.enabled", false);
    m_pShaderProgram->setUniformValue("objectMaterial.usedTexture", true);
    m_pShaderProgram->setUniformValue("objectFactor", QVector3D(0.2f, 0.2f, 0.2f));

    // 绘制盒子
    drawBox(QVector3D(0.0f, -2.5f, 0.0f), true);
    drawBox(QVector3D(2.0f, -2.5f, 2.0f), true);
}

void CubemapsRenderWidget::drawScaledTwoBox(void)
{
    m_pLightShaderProgram->bind();

    // 绘制盒子
    drawScaledBox(QVector3D(0.0f, -1.5f, 0.0f));
    drawScaledBox(QVector3D(2.0f, -1.5f, 2.0f));

    m_pLightShaderProgram->release();
}

void CubemapsRenderWidget::initModelData2(void)
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

void CubemapsRenderWidget::initModelData3(void)
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

// 天空盒
void CubemapsRenderWidget::initSkyBox(QOpenGLFunctions* f)
{
    m_pSkeyCubTexture = new COpenGLCubeTexture(this);
    m_pSkyBox = new COpenGLSkyBox(f, this);

//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeRight, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeRight, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeLeft, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeTop, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeButtom, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeFront, QImage(":/14_DepthTest/image/container2.png"));
//    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeBack, QImage(":/14_DepthTest/image/container2.png"));

    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeRight, QImage(":/20_Cubemaps/skybox/right.jpg"));
    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeLeft, QImage(":/20_Cubemaps/skybox/left.jpg"));
    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeTop, QImage(":/20_Cubemaps/skybox/top.jpg"));
    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeButtom, QImage(":/20_Cubemaps/skybox/bottom.jpg"));
    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeFront, QImage(":/20_Cubemaps/skybox/back.jpg"));
    m_pSkeyCubTexture->setImage(COpenGLCubeTexture::t_CubeBack, QImage(":/20_Cubemaps/skybox/front.jpg"));
    m_pSkeyCubTexture->create();

    m_pSkyBox->setCubeTexture(m_pSkeyCubTexture);
    m_pSkyBox->setCamera(m_pCamera);
}

void CubemapsRenderWidget::drawSkyBox(void)
{
    m_pSkyBox->draw();
}

// 折射和反射
void CubemapsRenderWidget::initReflectionBox(QOpenGLFunctions* f)
{
//    m_pReflectionMesh = new COpenGLMesh(f, m_pReflectionAndRefractionShaderProgram, this);
//    initModelData(m_pReflectionMesh);

    m_pModel = new COpenGLModel(f, m_pReflectionAndRefractionShaderProgram, this);
    m_pModel->loadModel("./planet/planet.obj");
}

void CubemapsRenderWidget::initRefractionBox(QOpenGLFunctions* f)
{
    m_pRefractionMesh = new COpenGLMesh(f, m_pReflectionAndRefractionShaderProgram, this);
    initModelData(m_pRefractionMesh);
}

void CubemapsRenderWidget::drawReflectionBox(void)
{
    m_pReflectionAndRefractionShaderProgram->bind();

    for (int i=0; i<2; ++i)
    {
        // 设置模型矩阵
        QMatrix4x4 modelMat;
        if (i == 0)
        {
            modelMat.translate(0.0f, 0.0f, 0.0f);
            m_pReflectionAndRefractionShaderProgram->setUniformValue("M_isRefection", true);
        }
        else
        {
            modelMat.translate(2.0f, 0.0f, 2.0f);
            m_pReflectionAndRefractionShaderProgram->setUniformValue("M_isRefection", false);
        }
        modelMat.scale(0.2f, 0.2f, 0.2f);
        m_pReflectionAndRefractionShaderProgram->setUniformValue("M", modelMat);

        // 获取相机
        m_pCamera->activeCamera(m_pReflectionAndRefractionShaderProgram);
        m_pReflectionAndRefractionShaderProgram->setUniformValue("M_EyePostion", m_pCamera->getCameraPostion());

        // 设置Cube贴图
        glActiveTexture(GL_TEXTURE0);
        m_pSkyBox->getCubeTexture()->bind();
        m_pReflectionAndRefractionShaderProgram->setUniformValue("skyBoxCube", 0);

    //    m_pReflectionMesh->draw();
        m_pModel->setTextureRenderEnabled(false);
        m_pModel->draw();
    }

    m_pReflectionAndRefractionShaderProgram->release();
}

void CubemapsRenderWidget::drawRefractionBox(void)
{

}

void CubemapsRenderWidget::initFloor(QOpenGLFunctions* f)
{
    m_pMeshFloor = new COpenGLMesh(f, m_pShaderProgram, this);

    // 添加地板紋理
//    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
//    pMeshTexture->setFilterType(COpenGLTexture::t_nearest);
//    pMeshTexture->create();
//    pMeshTexture->setImage(":/14_DepthTest/image/metal.png");
//    pMeshTexture->setType(COpenGLTexture::t_diffuse);
//    m_pMeshFloor->addTexture(pMeshTexture);

    // 設置地板的顶点数据
    initModelData2();
}

void CubemapsRenderWidget::drawFloor(void)
{
    QMatrix4x4 mat;
    mat.translate(QVector3D(0.0f, -2.0f, 0.0f));
    mat.scale(15.0f, 15.0f, 15.0f);
    m_pShaderProgram->setUniformValue("M", mat);
    m_pShaderProgram->setUniformValue("lightMaterial.enabled", true);

//    QVector3D colorVec(0.0f / 255, 130.0f / 255, 130.0f / 255);
//    QVector3D colorVec(128.0f / 255, 187.0f / 255, 253.0f / 255);
    QVector3D colorVec(50.0f / 255, 50.0f / 255, 50.0f / 255);
    m_pShaderProgram->setUniformValue("objectMaterial.ambientColor", colorVec);
    m_pShaderProgram->setUniformValue("objectMaterial.diffuseColor", colorVec);
    m_pShaderProgram->setUniformValue("objectFactor", QVector3D(1.0f, 1.0f, 1.0f));
    m_pShaderProgram->setUniformValue("objectMaterial.usedTexture", false);

    m_pMeshFloor->draw();
}

// 草
void CubemapsRenderWidget::initGrass(QOpenGLFunctions* f)
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

void CubemapsRenderWidget::initCurrentFBO(QOpenGLFunctions* f)
{
    m_pCurrentFBOMesh = new COpenGLMesh(f, m_pCurrentFBOShaderProgram, this);
    initCurrentFBOData(m_pCurrentFBOMesh);
}

void CubemapsRenderWidget::initCurrentFBO2(QOpenGLFunctions* f)
{
    m_pCurrentFBOMesh2 = new COpenGLMesh(f, m_pCurrentFBOShaderProgram, this);
    initCurrentFBOData2(m_pCurrentFBOMesh2);
}

void CubemapsRenderWidget::initCurrentFBOData(COpenGLMesh* pMesh)
{
    CAttributePointArray arrays;

    float vertices[] = {
            // positions          // normals           // texture coords
            -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f
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

void CubemapsRenderWidget::initCurrentFBOData2(COpenGLMesh* pMesh)
{
    CAttributePointArray arrays;

    float vertices[] = {
            // positions          // normals           // texture coords
           0.25f, 1.0f / 4.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             1.0f, 1.0f / 4.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.25f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            0.25f, 1.0f / 4.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f
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

void CubemapsRenderWidget::initGrassModelData(COpenGLMesh* pMesh)
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

void CubemapsRenderWidget::drawGrass(void)
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

void CubemapsRenderWidget::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(30);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &CubemapsRenderWidget::onTimeout);
    m_pTimer->start();
}

void CubemapsRenderWidget::onTimeout(void)
{
    m_angle += 2.0;

    // 绘制到一个FBO中
    this->makeCurrent();
    m_pFBO->bind();
    glDisable(GL_CULL_FACE);
    drawScene();
    m_pFBO->unbind();

    this->update();
}

// 设置面剔除相关
void CubemapsRenderWidget::setCullFaceEnabled(bool isEnabled)
{
    if (m_isCullFaceEnabled == isEnabled)
        return;

    m_isCullFaceEnabled = isEnabled;
    this->makeCurrent();
    if (m_isCullFaceEnabled)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    this->update();
}

bool CubemapsRenderWidget::isCullFaceEnabled(void)
{
    return m_isCullFaceEnabled;
}

// 设置要剔除的面
void CubemapsRenderWidget::setCullFaceType(CullFaceType type)
{
    if (m_faceType == type)
        return;

    m_faceType = type;
    this->makeCurrent();
    switch (m_faceType) {
        case t_back:
            glCullFace(GL_BACK);
            break;
        case t_front:
            glCullFace(GL_FRONT);
            break;
        case t_frontAndBack:
            glCullFace(GL_FRONT_AND_BACK);
            break;

    default:
        break;
    }

    this->update();
}

CubemapsRenderWidget::CullFaceType CubemapsRenderWidget::getCullFaceType(void)
{
    return m_faceType;
}

// 设置正面的顺序
void CubemapsRenderWidget::setFrontOrderType(FrontFaceOrderType type)
{
    if (m_frontOrderType == type)
        return;

    m_frontOrderType = type;
    this->makeCurrent();
    switch (m_frontOrderType) {
        case t_CCW:
            glFrontFace(GL_CCW);
            break;
        case t_CW:
            glFrontFace(GL_CW);
            break;
    default:
        break;
    }

    this->update();
}

CubemapsRenderWidget::FrontFaceOrderType CubemapsRenderWidget::getFrontOrderType(void)
{
    return m_frontOrderType;
}
