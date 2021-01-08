#ifndef UICUSTOMVEC3WIDGET_H
#define UICUSTOMVEC3WIDGET_H

#include <QWidget>
#include <QVector3D>

class UICustomDoubleSpinBox;
class QHBoxLayout;
class UICustomVec3Widget : public QWidget
{
    Q_OBJECT

public:
    UICustomVec3Widget(QWidget* parent = nullptr);
    ~UICustomVec3Widget();

    // 设置和获取数值
    void setValue(const QVector3D& vec);
    QVector3D getValue(void);

    // 设置间隔
    void setSpinBoxSpacing(int spacing);
    // 设置Margin
    void setSpinBoxMargin(const QMargins& margins);

private:
    QVector3D m_value;

    UICustomDoubleSpinBox* m_pDoubleSpinBoxX = nullptr;
    UICustomDoubleSpinBox* m_pDoubleSpinBoxY = nullptr;
    UICustomDoubleSpinBox* m_pDoubleSpinBoxZ = nullptr;

    QHBoxLayout* m_pMainLayout = nullptr;

    void updateDisplay(void);
    void updateValue(void);

signals:
    void valueChanged(const QVector3D& vec);

private slots:
    void onDoubleSpinBoxValueChanged(double value);
};

#endif
