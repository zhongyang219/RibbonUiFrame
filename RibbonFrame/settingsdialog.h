#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Data
    {
        bool ribbonHideEnable{};            //是否允许隐藏功能区
        bool ribbonPin{ true };             //是否固定功能区
        bool ribbonDoubleClickEnable{};     //是否允许通过双击Ribbon标签显示/隐藏功能区

        //功能区隐藏时，在点击Ribbon标签时显示什么
        enum ShowWhenTabClicked
        {
            None,       //无
            Ribbon,     //显示功能区
            Menu        //显示菜单
        };
        ShowWhenTabClicked showWhenTabClicked{};

        bool showLeftNaviBar{};             //是否在左侧显示导航栏

        void Save() const;
        void Load();
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void SetData(Data data);
    Data GetData() const;

private slots:
    void on_enableHideRibbonCheck_stateChanged(int arg1);

private:
    void EnableControl();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
