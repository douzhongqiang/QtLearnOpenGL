#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class UICustomLeftTabsControl;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    UICustomLeftTabsControl* m_pLeftControl = nullptr;

    void initUI(void);
};

#endif // MAINWINDOW_H
