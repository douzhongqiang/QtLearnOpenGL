#ifndef CAMERAROAMWIDGET_H
#define CAMERAROAMWIDGET_H

#include <QWidget>

class CameraRoamRenderWidget;
class UICustomVec3Control;
class UICustomDoubleControl;
class CameraRoamWidget : public QWidget
{
    Q_OBJECT

public:
    CameraRoamWidget(QWidget* parent = nullptr);
    ~CameraRoamWidget();

private:
    CameraRoamRenderWidget* m_pRenderWidget = nullptr;

    UICustomVec3Control* m_pPosVec3Control = nullptr;
    UICustomVec3Control* m_pFrontVec3Control = nullptr;
    UICustomDoubleControl* m_pPerspAngleControl = nullptr;

private slots:
    void onCameraPostionChanged(const QVector3D& vec);
    void onCameraFrontChanged(const QVector3D& vec);
    void onProjMatrixAngleChanged(float angle);

    void onCameraPostionControlValueChanged(const QVector3D& vec);
    void onCameraFrontControlValueChanged(const QVector3D& vec);
    void onProjMatrixAngleControlValueChanged(qreal value, bool cmd);
};

#endif
