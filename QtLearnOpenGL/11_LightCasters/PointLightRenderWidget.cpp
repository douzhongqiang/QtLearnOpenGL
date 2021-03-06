#include "PointLightRenderWidget.h"
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

PointLightRenderWidget::PointLightRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    initObjectMaterial();
    initLightInfo();
}

PointLightRenderWidget::~PointLightRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void PointLightRenderWidget::initializeGL()
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

    // 添加漫反射贴图
    m_diffuseTexture = new COpenGLTexture(f, this);
    m_pRender->addTexture(m_diffuseTexture);
    m_diffuseTexture->create();
    m_diffuseTexture->setImage(QImage(":/10_LightingMaps/image/container2.png"));
//    m_diffuseTexture->setImage(QImage(":/10_LightingMaps/image/uqahruewcx.jpeg"));
    m_diffuseTexture->setName("objectMaterial.diffuse");

    m_specularTexture = new COpenGLTexture(f, this);
    m_pRender->addTexture(m_specularTexture);
    m_specularTexture->create();
    m_specularTexture->setImage(QImage(":/10_LightingMaps/image/container2_specular.png"));
//    m_specularTexture->setImage(QImage(":/10_LightingMaps/image/lighting_maps_specular_color.png"));
    m_specularTexture->setName("objectMaterial.specular");

    // 设置节点数据
    if (!m_pVertexObject)
        m_pVertexObject = new COpenGLVertexObject(f, this);
    m_pRender->addVertexDataObject(m_pVertexObject);

    // 设置相机
    if (!m_pCamera)
        m_pCamera = new COpenGLCamera(f, this);
    m_pCamera->setCameraShaderName("V", "P");
    m_pRender->setCamera(m_pCamera);
    m_pCamera->setCameraPostion(QVector3D(0.0f, 0.0f, 5.0f));

    // 获取位置和颜色的locationID
    m_pVertexObject->setName("pos", "texCoord", "normal");
    // 创建顶点属性数据
    initModelData();

    m_pRender->init();

    m_MMat.translate(QVector3D(0, 0, -20));
}

void PointLightRenderWidget::resizeGL(int w, int h)
{
    m_pRender->resize(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void PointLightRenderWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    // 设置光的信息
    m_pShaderProgram->setUniformValue("lightMaterial.lightPos", m_light.lightPostion);
    // 光的材质信息
    m_pShaderProgram->setUniformValue("lightMaterial.ambient", m_light.ambientColor);
    m_pShaderProgram->setUniformValue("lightMaterial.diffuse", m_light.diffuesColor);
    m_pShaderProgram->setUniformValue("lightMaterial.specular", m_light.specularColor);
    // 光的衰减系数
    m_pShaderProgram->setUniformValue("lightMaterial.constant", m_light.constant);
    m_pShaderProgram->setUniformValue("lightMaterial.linear", m_light.linear);
    m_pShaderProgram->setUniformValue("lightMaterial.quadratic", m_light.quadratic);

    // 設置眼睛的位置
    if (m_pCamera)
    {
        QVector3D cameraPos = m_pCamera->getCameraPostion();
        m_pShaderProgram->setUniformValue("M_ViewPostion", cameraPos);
    }

    // 绘制立方体
    // =============================================================

    // 设置物体的材质
//    m_pShaderProgram->setUniformValue("objectMaterial.ambient", m_objectMaterial.ambientColor);
//    m_pShaderProgram->setUniformValue("objectMaterial.diffuse", m_objectMaterial.diffuesColor);
//    m_pShaderProgram->setUniformValue("objectMaterial.specular", m_objectMaterial.specularColor);
    m_pShaderProgram->setUniformValue("objectMaterial.shininess", m_objectMaterial.shininess);

    for (int i=0; i<m_transPos.size(); ++i)
    {
        m_MMat.setToIdentity();
        m_MMat.translate(m_transPos[i]);

        float angle = 20.0f * i;
        m_MMat.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));

        // 设置矩阵
        m_pShaderProgram->setUniformValue("M", m_MMat);

        m_pRender->render();
    }
    // =============================================================

    m_pShaderProgram->release();
}

void PointLightRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void PointLightRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void PointLightRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void PointLightRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void PointLightRenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool PointLightRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/11_LightCasters/shader/PointLightVertexShader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/11_LightCasters/shader/PointLightFragmentShader.fsh");
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

void PointLightRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool PointLightRenderWidget::isFill(void)
{
    return m_isFill;
}

// 设置/获取物体的材质
void PointLightRenderWidget::setObjectMaterial(const ObjectMaterial& material)
{
    m_objectMaterial = material;
    this->update();
}

PointLightRenderWidget::ObjectMaterial PointLightRenderWidget::getObjectMaterial(void)
{
    return m_objectMaterial;
}

// 设置/获取光的属性信息
void PointLightRenderWidget::setLightInfo(const LightInfo& info)
{
    m_light = info;

    m_light.constant = 1.0f;
    m_light.linear = 0.09f;
    m_light.quadratic = 0.032f;

    this->update();
}

PointLightRenderWidget::LightInfo PointLightRenderWidget::getLightInfo(void)
{
    return m_light;
}

void PointLightRenderWidget::initObjectMaterial(void)
{
//    m_objectMaterial.ambientColor = QVector3D(0.0f, 0.1f, 0.06f);
//    m_objectMaterial.diffuesColor = QVector3D(0.0f, 0.50980392f, 0.50980392f);
//    m_objectMaterial.specularColor = QVector3D(0.50196078f, 0.50196078f, 0.50196078f);
    m_objectMaterial.shininess = 32.0f;

    this->update();
}

void PointLightRenderWidget::initLightInfo(void)
{
    m_light.lightPostion = QVector3D(2.6f, 0.0f, -0.5f);

    m_light.ambientColor = QVector3D(0.2f, 0.2f, 0.2f);
    m_light.diffuesColor = QVector3D(0.5f, 0.5f, 0.5f);
    m_light.specularColor = QVector3D(1.0f, 1.0f, 1.0f);

    m_light.constant = 1.0f;
    m_light.linear = 0.09f;
    m_light.quadratic = 0.032f;

    this->update();
}

void PointLightRenderWidget::initModelData(void)
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

    }
    m_pVertexObject->setPoints(arrays);
    m_pVertexObject->setType(COpenGLVertexObject::t_Triangle);

    m_transPos << QVector3D( 0.0f,  0.0f,  0.0f) << QVector3D( 2.0f,  5.0f, -15.0f) \
               << QVector3D( -1.5f, -2.2f, -2.5f) << QVector3D( -3.8f, -2.0f, -12.3f) \
               << QVector3D( 2.4f, -0.4f, -3.5f) << QVector3D( -1.7f,  3.0f, -7.5f) \
               << QVector3D( 1.3f, -2.0f, -2.5f) << QVector3D( 1.5f,  2.0f, -2.5f) \
               << QVector3D( 1.5f,  0.2f, -1.5f) << QVector3D( -1.3f,  1.0f, -1.5f) ;
}
