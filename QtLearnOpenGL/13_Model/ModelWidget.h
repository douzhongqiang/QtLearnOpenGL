#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QWidget>

class ModelRenderWidget;
class ModelAttrNode;

class ModelWidget : public QWidget
{
    Q_OBJECT

public:
    ModelWidget(QWidget* parent = nullptr);
    ~ModelWidget();

private:
    ModelRenderWidget* m_pRenderWidget = nullptr;
    ModelAttrNode* m_pMaterialNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onClickedButton(void);
};

#endif
