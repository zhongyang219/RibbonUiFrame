#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ribbonframewindow.h"
#include "GenerateResourceIdDefines/widget.h"

class MainWindow : public RibbonFrameWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void HideTabArea();

private:

    QTabWidget* m_pTabWidget;
    GenerateResourceIdDefines::Widget m_generateResIdDeninesWidget;
    // IMainFrame interface
public:
    virtual void *SendModuleMessage(const char *moduleName, const char *msgType, void *para1, void *para2) override;
};
#endif // MAINWINDOW_H
