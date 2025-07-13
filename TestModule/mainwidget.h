#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace Ui {
class CMainWidget;
}

class CMainWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWidget(QMainWindow *parent = nullptr);
    ~CMainWidget();

    QWidget* GetLeftPanel() const;
    QWidget* GetRightPanel() const;
    QWidget* GetBottomPanel() const;
    QMenuBar* GetMenuBar() const;

private:
    void InitSytemIcon();

private slots:
    void on_btnInfo_clicked();

    void on_btnQuestion_clicked();

    void on_btnError_clicked();

    void on_btnInput_clicked();

    void on_btnFont_clicked();

    void on_btnColor_clicked();

    void OnIconBtnClicked(bool);

    void OnNavChanged(const QString& text);

private:
    Ui::CMainWidget *ui;
};

#endif // MAINWIDGET_H
