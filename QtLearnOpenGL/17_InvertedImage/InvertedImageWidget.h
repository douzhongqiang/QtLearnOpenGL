#ifndef INVERTEDIMAGEWIDGET_H
#define INVERTEDIMAGEWIDGET_H

#include <QWidget>

class InvertedImageRenderWidget;
class InvertedImageAttrNode;
class QPushButton;

class InvertedImageWidget : public QWidget
{
    Q_OBJECT

public:
    InvertedImageWidget(QWidget* parent = nullptr);
    ~InvertedImageWidget();

private:
    InvertedImageRenderWidget* m_pRenderWidget = nullptr;
    InvertedImageAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
