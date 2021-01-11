#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>

class CubeRenderWidget;
class UICustomDoubleControl;
class UICustomVec3Control;
class UICustomIntControl;
class CubeWidget : public QWidget
{
    Q_OBJECT

public:
    CubeWidget(QWidget* parent = nullptr);
    ~CubeWidget();

private:
    CubeRenderWidget* m_pRenderWidget = nullptr;
    UICustomDoubleControl* m_pMinNumberControl = nullptr;
    UICustomVec3Control* m_pVecControl = nullptr;
    UICustomIntControl* m_pSpeedControl = nullptr;

private slots:
    void onValueChanged(qreal value, bool cmd);
    void onVecValueChanged(const QVector3D& vec);
    void onSpeedChanged(int speed);
};

#endif
