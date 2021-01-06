#ifndef IBOANDPOLYGONMODE_H
#define IBOANDPOLYGONMODE_H

#include <QWidget>

class IBOAndPolygonModeWidget;
class IBOAndPolygonMode : public QWidget
{
    Q_OBJECT

public:
    IBOAndPolygonMode(QWidget* parent = nullptr);
    ~IBOAndPolygonMode();

private:
    IBOAndPolygonModeWidget* m_pRenderWidget = nullptr;

private slots:
    void onClickedButton(void);
};

#endif
