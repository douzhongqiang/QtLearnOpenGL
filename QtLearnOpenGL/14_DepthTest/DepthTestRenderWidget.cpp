#include "DepthTestRenderWidget.h"
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

DepthTestRenderWidget::DepthTestRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    initLightInfo();
}

DepthTestRenderWidget::~DepthTestRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void DepthTestRenderWidget::initializeGL()
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

    // 设置相机
    if (!m_pCamera)
        m_pCamera = new COpenGLCamera(f, this);
    m_pCamera->setCameraShaderName("V", "P");
    m_pCamera->setCameraPostion(QVector3D(0.0f, 0.0f, 5.0f));
    m_pCamera->setShaderProgram(m_pShaderProgram);


    initFloor(f);

    initTimer();
}

void DepthTestRenderWidget::resizeGL(int w, int h)
{
    m_pCamera->setViewport(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void DepthTestRenderWidget::paintGL()
{
    glClearColor(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

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
    m_pCamera->activeCamera();

    m_MMat.setToIdentity();
//    m_MMat.translate(QVector3D(0.0f, -2.0f, 0.0f));
//    m_MMat.translate(QVector3D(0.0f, 0.0f, -2.0f));
//    m_MMat.rotate(m_angle, QVector3D(1.0f, 0.0f, 0.0f));
//    m_MMat.scale(3.0f, 3.0f, 3.0f);

    // 设置模型矩阵
//    m_pShaderProgram->setUniformValue("M", m_MMat);

    // 绘制地板
    drawFloor();

    m_pShaderProgram->release();
}

void DepthTestRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pCamera)
        m_pCamera->keyPressEvent(event);

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void DepthTestRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mousePressEvent(event);

    return QOpenGLWidget::mousePressEvent(event);
}

void DepthTestRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseMoveEvent(event);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void DepthTestRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pCamera)
        m_pCamera->mouseReleaseEvent(event);

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void DepthTestRenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pCamera)
        m_pCamera->wheelEvent(event);

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool DepthTestRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

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

void DepthTestRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool DepthTestRenderWidget::isFill(void)
{
    return m_isFill;
}

// 设置/获取光的属性信息
void DepthTestRenderWidget::setLightInfo(const LightInfo& info)
{
    m_light = info;
    this->update();
}

DepthTestRenderWidget::LightInfo DepthTestRenderWidget::getLightInfo(void)
{
    return m_light;
}

void DepthTestRenderWidget::initLightInfo(void)
{
    m_light.lightPos = QVector3D(0.0f, 0.0f, -1.0f);

    m_light.ambientColor = QVector3D(0.2f, 0.2f, 0.2f);
    m_light.diffuesColor = QVector3D(0.5f, 0.5f, 0.5f);
    m_light.specularColor = QVector3D(1.0f, 1.0f, 1.0f);

    this->update();
}

void DepthTestRenderWidget::initBox(QOpenGLFunctions* f)
{
    m_pMesh = new COpenGLMesh(f, m_pShaderProgram, this);

    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->create();
    pMeshTexture->setImage(":/10_LightingMaps/image/container2.png");
    pMeshTexture->setType(COpenGLTexture::t_diffuse);
    m_pMesh->addTexture(pMeshTexture);

    COpenGLTexture* pMeshTexture2 = new COpenGLTexture(f, this);
    pMeshTexture2->create();
    pMeshTexture2->setImage(":/10_LightingMaps/image/container2_specular.png");
    pMeshTexture2->setType(COpenGLTexture::t_specular);
    m_pMesh->addTexture(pMeshTexture2);

    initModelData();
}

void DepthTestRenderWidget::initModelData(void)
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
    m_pMesh->setPoints(arrays);
    m_pMesh->setIndices(vecs);

    m_pMesh->setupMesh();
}

void DepthTestRenderWidget::initModelData2(void)
{
    CAttributePointArray arrays;

//    float vertices[] = {
//            // positions          // normals           // texture coords
//            -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//             0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//             0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

//             0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//            -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//            -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
//        };

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
    m_pMeshFloor->setPoints(arrays);
    m_pMeshFloor->setIndices(vecs);

    m_pMeshFloor->setupMesh();
}

void DepthTestRenderWidget::initModelData3(void)
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

void DepthTestRenderWidget::initFloor(QOpenGLFunctions* f)
{
    m_pMeshFloor = new COpenGLMesh(f, m_pShaderProgram, this);

    // 添加地板紋理
    COpenGLTexture* pMeshTexture = new COpenGLTexture(f, this);
    pMeshTexture->create();
    pMeshTexture->setImage(":/14_DepthTest/image/container2.png");
    pMeshTexture->setType(COpenGLTexture::t_diffuse);
    m_pMeshFloor->addTexture(pMeshTexture);

    COpenGLTexture* pMeshTexture2 = new COpenGLTexture(f, this);
    pMeshTexture2->create();
    pMeshTexture2->setImage(":/10_LightingMaps/image/container2_specular.png");
    pMeshTexture2->setType(COpenGLTexture::t_specular);
    m_pMeshFloor->addTexture(pMeshTexture2);

    // 設置地板的顶点数据
    initModelData2();
}

void DepthTestRenderWidget::drawFloor(void)
{
    QMatrix4x4 mat;
    mat.scale(10.0f, 10.0f, 10.0f);
    m_pShaderProgram->setUniformValue("M", mat);

    m_pMeshFloor->draw();
}

void DepthTestRenderWidget::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(50);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &DepthTestRenderWidget::onTimeout);
    m_pTimer->start();
}

void DepthTestRenderWidget::onTimeout(void)
{
    m_angle += 2.0;
//    qDebug() << __FUNCTION__ << m_angle;

    this->update();
}
