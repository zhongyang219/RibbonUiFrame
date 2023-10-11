#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ribbonframewindow.h"
#include <QTextEdit>

class MainWindow : public RibbonFrameWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitStatusBar();

private:
    QTextEdit m_edit;

    // RibbonFrameWindow interface
protected:
    virtual bool OnCommand(const QString &strCmd, bool checked) override;
    virtual QWidget *CreateUserWidget(const QString &strId, QWidget *pParent) override;
};
#endif // MAINWINDOW_H
