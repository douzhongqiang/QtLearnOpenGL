#ifndef CUBEMAPSWIDGET_H
#define CUBEMAPSWIDGET_H

#include <QWidget>

class CubemapsRenderWidget;
class CubemapsAttrNode;
class QPushButton;

class CubemapsWidget : public QWidget
{
    Q_OBJECT

public:
    CubemapsWidget(QWidget* parent = nullptr);
    ~CubemapsWidget();

private:
    CubemapsRenderWidget* m_pRenderWidget = nullptr;
    QPushButton* m_pButton = nullptr;
    CubemapsAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
