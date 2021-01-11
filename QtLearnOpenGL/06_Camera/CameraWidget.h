#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

class CameraRenderWidget;
class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    CameraWidget(QWidget* parent = nullptr);
    ~CameraWidget();

private:
    CameraRenderWidget* m_pRenderWidget = nullptr;
};

#endif
