#include "LightBaseRenderWidget.h"
#include "OpenGLEngine/COpenGLTexture.h"
#include "OpenGLEngine/COpenGLRender.h"
#include "OpenGLEngine/COpenGLVertexObject.h"
#include "OpenGLEngine/CAttributePoint.h"
#include "OpenGLEngine/COpenGLCamera.h"
#include "Utils.h"
#include <QtMath>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

LightBaseRenderWidget::LightBaseRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
    , m_cObjectColor(200, 100, 100)
    , m_cLightColor(255, 255, 255)
    , m_lightPostion(1.2f, 0.5f, 2.0f)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
}

LightBaseRenderWidget::~LightBaseRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void LightBaseRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化GPU程序
    bool result = initShaderProgram();
    if (!result)
        return;

    glEnable(GL_DEPTH_TEST);

    m_shaderProgramId = m_pShaderProgram->programId();

    // 设置纹理等
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

    if (!m_pRender)
        m_pRender = new COpenGLRender(f, this);
    m_pRender->setShaderProgram(m_pShaderProgram);

    // 设置节点数据
    if (!m_pVertexObject)
        m_pVertexObject = new COpenGLVertexObject(f, this);
    m_pRender->addVertexDataObject(m_pVertexObject);

    // 设置相机
    if (!m_pCamera)
        m_pCamera = new COpenGLCamera(f, this);
    m_pCamera->setCameraShaderName("V", "P");
    m_pRender->setCamera(m_pCamera);
    m_pCamera->setCameraPostion(QVector3D(0.0f, 0.0f, 10.0f));

    // 获取位置和颜色的locationID
    m_pVertexObject->setName("pos", "texCoord", "normal");
    // 创建顶点属性数据
    initModelData();

    m_pRender->init();

    // 设置物体的颜色
    QVector3D objectVec = g_utilTool->color2Vec3(m_cObjectColor);
    m_pShaderProgram->setUniformValue("M_ObjecColor", objectVec);

    m_MMat.translate(QVector3D(0, 0, -20));
}

void LightBaseRenderWidget::resizeGL(int w, int h)
{
    m_pRender->resize(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void LightBaseRenderWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    // 设置光的颜色
    QVector3D lightColorVec = g_utilTool->color2Vec3(m_cLightColor);
    m_pShaderProgram->setUniformValue("M_LightColor", lightColorVec);
    // 设置光的位置
    m_pShaderProgram->setUniformValue("M_LightPostion", m_lightPostion);
    // 設置眼睛的位置
    if (m_pCamera)
    {
        QVector3D cameraPos = m_pCamera->getCameraPostion();
        m_pShaderProgram->setUniformValue("M_ViewPostion", cameraPos);
    }

    // 绘制光源
    // =============================================================
    // 设置物体的颜色
    QVector3D ligthtVec = g_utilTool->color2Vec3(m_cLightColor);
    m_pShaderProgram->setUniformValue("M_ObjecColor", ligthtVec);

    m_MMat.setToIdentity();
    m_MMat.translate(m_lightPostion);

    m_MMat.scale(QVector3D(0.05f, 0.05f, 0.05f));

    // 设置矩阵
    m_pShaderProgram->setUniformValue("M", m_MMat);

    m_pRender->render();
    // =============================================================

    // 绘制立方体
    // =============================================================
    // 设置物体的颜色
    QVector3D objectVec = g_utilTool->color2Vec3(m_cObjectColor);
    m_pShaderProgram->setUniformValue("M_ObjecColor", objectVec);

    m_MMat.setToIdentity();
    m_MMat.translate(m_transPos[0]);
    m_MMat.rotate(30, QVector3D(0.0f, 1.0f, 0.0f));

    // 设置矩阵
    m_pShaderProgram->setUniformValue("M", m_MMat);

    m_pRender->render();
    // =============================================================

    m_pShaderProgram->release();
}

void LightBaseRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void LightBaseRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void LightBaseRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void LightBaseRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void LightBaseRenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool LightBaseRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/08_LightBase/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/08_LightBase/shader/fragmentshader.fsh");
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

void LightBaseRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool LightBaseRenderWidget::isFill(void)
{
    return m_isFill;
}

// 设置/获取物体的颜色
void LightBaseRenderWidget::setObjectColor(const QColor& color)
{
    m_cObjectColor = color;
    this->update();
}

QColor LightBaseRenderWidget::getObjectColor(void)
{
    return m_cObjectColor;
}

// 设置/获取光的颜色
void LightBaseRenderWidget::setLightColor(const QColor& color)
{
    m_cLightColor = color;
    this->update();
}

QColor LightBaseRenderWidget::getLightColor(void)
{
    return m_cLightColor;
}

// 设置/获取光的位置
void LightBaseRenderWidget::setLightPostion(const QVector3D& pos)
{
    m_lightPostion = pos;
    this->update();
}

QVector3D LightBaseRenderWidget::getLightPostion(void)
{
    return m_lightPostion;
}

void LightBaseRenderWidget::initModelData(void)
{
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

    int count = sizeof(facePoint) / sizeof(int);
    CAttributePointArray arrays;

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
    }

    m_pVertexObject->setPoints(arrays);

    m_transPos << QVector3D( 0.0f,  0.0f,  0.0f) << QVector3D( 12.0f,  5.0f, -15.0f) \
               << QVector3D( -11.5f, -2.2f, -2.5f) << QVector3D( -13.8f, -8.0f, -12.3f) \
               << QVector3D( -1.3f,  5.0f, -1.5f) << QVector3D( 2.4f, -6.4f, -3.5f) \
               << QVector3D( -1.7f,  8.0f, -7.5f) << QVector3D( 11.3f, -2.0f, -2.5f) \
               << QVector3D( 11.5f,  2.0f, -2.5f) << QVector3D( 1.5f,  0.2f, -1.5f) ;
}
