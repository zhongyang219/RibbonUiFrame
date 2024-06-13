#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSettings>
#include "ribbonuipredefine.h"

void SettingsDialog::Data::Save() const
{
    //保存设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("ribbonHideEnable", static_cast<int>(ribbonHideEnable));
    settings.setValue("ribbonPin", static_cast<int>(ribbonPin));
    settings.setValue("ribbonDoubleClickEnable", static_cast<int>(ribbonDoubleClickEnable));
    settings.setValue("showWhenTabClicked", static_cast<int>(showWhenTabClicked));
}

void SettingsDialog::Data::Load()
{
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    ribbonHideEnable = settings.value("ribbonHideEnable", false).toBool();
    if (!ribbonHideEnable)
        ribbonPin = true;
    else
        ribbonPin = settings.value("ribbonPin", true).toBool();
    ribbonDoubleClickEnable = settings.value("ribbonDoubleClickEnable", false).toBool();
    showWhenTabClicked = static_cast<ShowWhenTabClicked>(settings.value("showWhenTabClicked", 0).toInt());
}

/////////////////////////////////////////////////////////////////////////////////
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    resize(DPI(330), DPI(230));
    ui->showWhenTabClickedComboBox->addItem(u8"无");
    ui->showWhenTabClickedComboBox->addItem(u8"显示功能区");
    ui->showWhenTabClickedComboBox->addItem(u8"显示菜单");
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::SetData(Data data)
{
    ui->enableHideRibbonCheck->setChecked(data.ribbonHideEnable);
    ui->ribbonPinCheck->setChecked(data.ribbonPin);
    ui->doubleClickShowHideRibbonCheck->setChecked(data.ribbonDoubleClickEnable);
    ui->showWhenTabClickedComboBox->setCurrentIndex(static_cast<int>(data.showWhenTabClicked));
    EnableControl();
}

SettingsDialog::Data SettingsDialog::GetData() const
{
    Data data;
    data.ribbonHideEnable = ui->enableHideRibbonCheck->isChecked();
    data.ribbonPin = ui->ribbonPinCheck->isChecked();
    data.ribbonDoubleClickEnable = ui->doubleClickShowHideRibbonCheck->isChecked();
    data.showWhenTabClicked = static_cast<Data::ShowWhenTabClicked>(ui->showWhenTabClickedComboBox->currentIndex());
    if (!data.ribbonHideEnable)
        data.ribbonPin = true;

    return data;
}

void SettingsDialog::EnableControl()
{
    bool enable = ui->enableHideRibbonCheck->isChecked();
    ui->ribbonPinCheck->setEnabled(enable);
    ui->doubleClickShowHideRibbonCheck->setEnabled(enable);
    ui->showWhenTabClickedComboBox->setEnabled(enable);
}

void SettingsDialog::on_enableHideRibbonCheck_stateChanged(int)
{
    EnableControl();
}

