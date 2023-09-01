#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class CMainWidget;
}

class CMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CMainWidget(QWidget *parent = nullptr);
    ~CMainWidget();

private slots:
    void on_btnInfo_clicked();

    void on_btnQuestion_clicked();

    void on_btnError_clicked();

    void on_btnInput_clicked();

    void on_btnFont_clicked();

    void on_btnColor_clicked();

private:
    Ui::CMainWidget *ui;
};

#endif // MAINWIDGET_H
