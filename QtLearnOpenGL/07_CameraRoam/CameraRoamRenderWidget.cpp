#include "CameraRoamRenderWidget.h"
#include "OpenGLEngine/COpenGLTexture.h"
#include "OpenGLEngine/COpenGLRender.h"
#include "OpenGLEngine/COpenGLVertexObject.h"
#include "OpenGLEngine/CAttributePoint.h"
#include <QtMath>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

CameraRoamRenderWidget::CameraRoamRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
    , m_cameraPos(0.0f, 0.0f,  3.0f)
    , m_cameraFront(0.0f, 0.0f, -1.0f)
    , m_cameraUp(0.0f, 1.0f,  0.0f)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
}

CameraRoamRenderWidget::~CameraRoamRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void CameraRoamRenderWidget::initializeGL()
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
    if (!m_pTexture1) m_pTexture1 = new COpenGLTexture(f, this);
    if (!m_pTexture2) m_pTexture2 = new COpenGLTexture(f, this);
    if (!m_pRender) m_pRender = new COpenGLRender(f, this);
    m_pRender->setShaderProgram(m_pShaderProgram);
    m_pRender->addTexture(m_pTexture1);
    m_pRender->addTexture(m_pTexture2);
    // 设置节点数据
    if (!m_pVertexObject) m_pVertexObject = new COpenGLVertexObject(f, this);
    m_pRender->addVertexDataObject(m_pVertexObject);

    // 创建第一个纹理
    m_pTexture1->create();
    m_pTexture1->setImage(":/03_Texture/image/TestImage1.jpg");
    m_pTexture2->setName("texture1");

    // 创建第二个纹理
    m_pTexture2->create();
    m_pTexture2->setImage(":/03_Texture/image/TestImage2.jpg");
    m_pTexture2->setName("texture2");

    // 获取位置和颜色的locationID
    m_pVertexObject->setName("pos", "texCoord", "normal");
    // 创建顶点属性数据
    initModelData();

    m_pRender->init();

    m_MMat.translate(QVector3D(0, 0, -20));
}

void CameraRoamRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    m_PMat.setToIdentity();
    m_PMat.perspective(m_persAngle, this->width() * 1.0 / this->height(), 0.1f, 100.0f);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void CameraRoamRenderWidget::paintGL()
{
    glClearColor(51.0f / 255.0f, 76.0f / 255.0f, 76.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    // 设置Camera
    m_VMat.setToIdentity();
    m_VMat.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    m_pShaderProgram->setUniformValue("V", m_VMat);

    // 设置投影矩阵
    m_PMat.setToIdentity();
    m_PMat.perspective(m_persAngle, this->width() * 1.0 / this->height(), 0.1f, 100.0f);
    m_pShaderProgram->setUniformValue("P", m_PMat);

    // 绘制
    for (int i = 0; i<m_transPos.size(); ++i)
    {
        m_MMat.setToIdentity();
        m_MMat.translate(m_transPos[i]);

        float angle = 20.0f * i;
        m_MMat.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));

        // 设置混色的参数
        m_mixNumber = i * 1.0 / m_transPos.size();
        m_pShaderProgram->setUniformValue("mixNumber", m_mixNumber);

        // 设置矩阵
        m_pShaderProgram->setUniformValue("M", m_MMat);

        m_pRender->render();
    }

    m_pShaderProgram->release();
}

void CameraRoamRenderWidget::keyPressEvent(QKeyEvent* event)
{
    float speed = 0.5;
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        m_cameraPos += speed * m_cameraFront;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        m_cameraPos -= speed * m_cameraFront;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        QVector3D rightVec = QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
        m_cameraPos -= speed * rightVec;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        QVector3D rightVec = QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
        m_cameraPos += speed * rightVec;

        emit cameraPostionChanged(m_cameraPos);
    }

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void CameraRoamRenderWidget::mousePressEvent(QMouseEvent* event)
{
    m_bPressed = true;
    m_startPos = event->pos();
    return QOpenGLWidget::mousePressEvent(event);
}

void CameraRoamRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bPressed)
        return QOpenGLWidget::mouseMoveEvent(event);

    static bool isFirst = true;
    if (isFirst)
    {
        isFirst = false;

    }

    float xoffset = event->x() - m_startPos.x();
    float yoffset = m_startPos.y() - event->y();
    m_startPos = event->pos();
    qDebug() << xoffset << ", " << yoffset;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

//    if(m_pitch > 89.0f)
//      m_pitch =  89.0f;
//    if(m_yaw < -89.0f)
//      m_yaw = -89.0f;

    QVector3D front;
    front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    front.setY(qSin(qDegreesToRadians(m_pitch)));
    front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
//    qDebug() << front.x() << front.y() << front.z();

    m_cameraFront = front.normalized();
    emit cameraFrontChanged(m_cameraFront);
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void CameraRoamRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_bPressed = false;
    return QOpenGLWidget::mouseReleaseEvent(event);
}

void CameraRoamRenderWidget::wheelEvent(QWheelEvent *event)
{
    float interval = -event->delta() / 100.0;
    m_persAngle += interval;
    if (m_persAngle < 1.0f)
        m_persAngle = 1.0f;
    if (m_persAngle > 45.0f)
        m_persAngle = 45.0f;

    emit projMatrixAngleChanged(m_persAngle);
    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

bool CameraRoamRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/04_Cube/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/04_Cube/shader/fragmentshader.fsh");
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

void CameraRoamRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool CameraRoamRenderWidget::isFill(void)
{
    return m_isFill;
}

void CameraRoamRenderWidget::setMixNumber(float number)
{
    m_mixNumber = number;
    this->update();
}

float CameraRoamRenderWidget::getMixNumber(void)
{
    return m_mixNumber;
}

QVector3D CameraRoamRenderWidget::getCameraPostion(void)
{
    return m_cameraPos;
}

void CameraRoamRenderWidget::setCameraPostion(const QVector3D& vec)
{
    m_cameraPos = vec;
    this->update();
}

QVector3D CameraRoamRenderWidget::getCameraFront(void)
{
    return m_cameraFront;
}

void CameraRoamRenderWidget::setCameraFront(const QVector3D& vec)
{
    m_cameraFront = vec;
    this->update();
}

// 获取投影矩阵的角度
float CameraRoamRenderWidget::getProjMatrixAngle(void)
{
    return m_persAngle;
}

void CameraRoamRenderWidget::setProjMatrixAngle(float angle)
{
    m_persAngle = angle;
    this->update();
}

void CameraRoamRenderWidget::initModelData(void)
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
