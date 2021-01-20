#ifndef COPENGLCAMERA_H
#define COPENGLCAMERA_H

#include <QObject>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMatrix4x4>

class COpenGLCamera : public QObject
{
    Q_OBJECT

public:
    COpenGLCamera(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLCamera();

    void setViewport(int width, int height);
    void setViewRange(float nNear, float nFar);

    // 设置
    void setShaderProgram(QOpenGLShaderProgram* pProgram);

    // 設置在shader中的名字
    void setCameraShaderName(const QString& viewMatName, const QString& projMatName);

    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);

    // 设置相机相关的信息
    void setCameraPostion(const QVector3D& cameraPos);
    void setCameraFront(const QVector3D& cameraFront);
    void setc(const QVector3D& cameraUp);

    QVector3D getCameraPostion(void);
    QVector3D getCameraCameraFront(void);
    QVector3D getCameraCameraUp(void);

    // 设置投影矩阵相关
    void setPersAngle(float angle);
    float getPerAngle(void);

    void activeCamera(void);

    QMatrix4x4 getVMatrix(void);
    QMatrix4x4 getPMatrix(void);

private:
    QString m_viewMatName;
    QString m_projectMatName;

    QVector3D m_cameraPos;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;

    float m_yaw = -90.0f;               // 偏航角，鼠标的X坐标的偏移量
    float m_pitch = 0.0f;               // 俯仰角，鼠标的Y坐标的偏移量

    float m_persAngle = 45.0f;          // 角度

    QMatrix4x4 m_VMat;
    QMatrix4x4 m_PMat;

    QOpenGLShaderProgram* m_pProgram = nullptr;

    QPoint m_startPos;
    bool m_bPressed = false;

    int m_nViewportWidth = 0;
    int m_nViewportHeight = 0;
    float m_nNear = 0.1f;
    float m_nFar = 100.0f;

    void reSetProjectMatrix(void);
    void reSetViewMatrix(void);

signals:
    void cameraPostionChanged(const QVector3D& cameraPos);
    void cameraFrontChanged(const QVector3D& front);
    void projMatrixAngleChanged(float angle);
};

#endif
