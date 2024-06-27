#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "ribbonframewindow.h"

class MainWindow : public RibbonFrameWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr, const QStringList& cmdLine = QStringList());

signals:

    // MainFrameWindow interface
private:
    virtual bool OnCommand(const QString &strCmd, bool checked) override;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
