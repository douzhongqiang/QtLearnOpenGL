#include "COpenGLCamera.h"
#include <QtMath>

COpenGLCamera::COpenGLCamera(QOpenGLFunctions* function, QObject* parent)
    :QObject(parent)
    , m_cameraPos(0.0f, 0.0f, 10.0f)
    , m_cameraFront(0.0f, 0.0f, -1.0f)
    , m_cameraUp(0.0f, 1.0f, 0.0f)
{

}

COpenGLCamera::~COpenGLCamera()
{

}

// 设置
void COpenGLCamera::setShaderProgram(QOpenGLShaderProgram* pProgram)
{
    m_pProgram = pProgram;
}

void COpenGLCamera::setCameraShaderName(const QString& viewMatName, const QString& projMatName)
{
    m_viewMatName = viewMatName;
    m_projectMatName = projMatName;
}

void COpenGLCamera::keyPressEvent(QKeyEvent* event)
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
}

void COpenGLCamera::mousePressEvent(QMouseEvent* event)
{
    m_bPressed = true;
    m_startPos = event->pos();
}

void COpenGLCamera::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bPressed)
        return;

    float xoffset = event->x() - m_startPos.x();
    float yoffset = m_startPos.y() - event->y();
    m_startPos = event->pos();
    qDebug() << xoffset << ", " << yoffset;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    QVector3D front;
    front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    front.setY(qSin(qDegreesToRadians(m_pitch)));
    front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));

    m_cameraFront = front.normalized();
    emit cameraFrontChanged(m_cameraFront);
}

void COpenGLCamera::mouseReleaseEvent(QMouseEvent* event)
{
    m_bPressed = false;
}

void COpenGLCamera::wheelEvent(QWheelEvent *event)
{
    float interval = -event->delta() / 100.0;
    m_persAngle += interval;
    if (m_persAngle < 1.0f)
        m_persAngle = 1.0f;
    if (m_persAngle > 45.0f)
        m_persAngle = 45.0f;

    emit projMatrixAngleChanged(m_persAngle);
}

// 设置相机相关的信息
void COpenGLCamera::setCameraPostion(const QVector3D& cameraPos)
{
    m_cameraPos = cameraPos;
}

void COpenGLCamera::setCameraFront(const QVector3D& cameraFront)
{
    m_cameraFront = cameraFront;
}

void COpenGLCamera::setc(const QVector3D& cameraUp)
{
    m_cameraUp = cameraUp;
}

QVector3D COpenGLCamera::getCameraPostion(void)
{
    return m_cameraPos;
}

QVector3D COpenGLCamera::getCameraFront(void)
{
    return m_cameraFront;
}

QVector3D COpenGLCamera::getCameraUp(void)
{
    return m_cameraUp;
}

// 设置投影矩阵相关
void COpenGLCamera::setPersAngle(float angle)
{
    m_persAngle = angle;
}

float COpenGLCamera::getPerAngle(void)
{
    return m_persAngle;
}

void COpenGLCamera::setViewport(int width, int height)
{
    m_nViewportWidth = width;
    m_nViewportHeight = height;
}

void COpenGLCamera::setViewRange(float nNear, float nFar)
{
    m_nNear = nNear;
    m_nFar = nFar;
}

void COpenGLCamera::reSetProjectMatrix(void)
{
    m_PMat.setToIdentity();
    m_PMat.perspective(m_persAngle, m_nViewportWidth * 1.0 / m_nViewportHeight, m_nNear, m_nFar);
}

void COpenGLCamera::reSetViewMatrix(void)
{
    m_VMat.setToIdentity();
    m_VMat.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

QMatrix4x4 COpenGLCamera::getVMatrix(void)
{
    return m_VMat;
}

QMatrix4x4 COpenGLCamera::getPMatrix(void)
{
    return m_PMat;
}

void COpenGLCamera::activeCamera(void)
{
    reSetProjectMatrix();
    reSetViewMatrix();

    if (m_pProgram)
    {
        m_pProgram->setUniformValue(m_viewMatName.toLocal8Bit().data(), m_VMat);
        m_pProgram->setUniformValue(m_projectMatName.toLocal8Bit().data(), m_PMat);
    }
}

void COpenGLCamera::activeCamera(QOpenGLShaderProgram* pProgram)
{
    reSetProjectMatrix();
    reSetViewMatrix();

    if (pProgram)
    {
        pProgram->setUniformValue(m_viewMatName.toLocal8Bit().data(), m_VMat);
        pProgram->setUniformValue(m_projectMatName.toLocal8Bit().data(), m_PMat);
    }
}
