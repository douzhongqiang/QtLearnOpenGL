#ifndef LIGHTBASEWIDGET_H
#define LIGHTBASEWIDGET_H

#include <QWidget>

class LightBaseRenderWidget;
class UICustomVec3Control;
class UICustomDoubleControl;
class LightBaseWidget : public QWidget
{
    Q_OBJECT

public:
    LightBaseWidget(QWidget* parent = nullptr);
    ~LightBaseWidget();

private:
    LightBaseRenderWidget* m_pRenderWidget = nullptr;

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
