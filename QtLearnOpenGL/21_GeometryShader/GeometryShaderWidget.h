#ifndef GEOMETRYSHADERWIDGET_H
#define GEOMETRYSHADERWIDGET_H

#include <QWidget>

class GeometryShaderRenderWidget;
class GeometryShaderRenderWidget2;
class GeometryShaderRenderWidget3;
class GeometryShaderRenderWidget4;

class GeometryShaderWidget : public QWidget
{
    Q_OBJECT

public:
    GeometryShaderWidget(QWidget* parent = nullptr);
    ~GeometryShaderWidget();

private:
    GeometryShaderRenderWidget* m_pRenderWidget = nullptr;
    GeometryShaderRenderWidget2* m_pRenderWidget2 = nullptr;
    GeometryShaderRenderWidget3* m_pRenderWidget3 = nullptr;
    GeometryShaderRenderWidget4* m_pRenderWidget4 = nullptr;
};

#endif
