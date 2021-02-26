#ifndef DRAWINSTANCEWIDGET_H
#define DRAWINSTANCEWIDGET_H

#include <QWidget>

class DrawInstaceRenderWidget;

class DrawInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    DrawInstanceWidget(QWidget* parent = nullptr);
    ~DrawInstanceWidget();

private:
    DrawInstaceRenderWidget* m_pRenderWidget = nullptr;
};

#endif
