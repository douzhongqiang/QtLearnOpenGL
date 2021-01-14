#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QWidget>

class MaterialRenderWidget;
class MaterialNode;

class MaterialWidget : public QWidget
{
    Q_OBJECT

public:
    MaterialWidget(QWidget* parent = nullptr);
    ~MaterialWidget();

private:
    MaterialRenderWidget* m_pRenderWidget = nullptr;
    MaterialNode* m_pMaterialNode = nullptr;

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
};

#endif
