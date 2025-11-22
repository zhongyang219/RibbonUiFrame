#pragma once

#include <QDialog>
#include "ui_modulemanagerdlg.h"
#include <QSet>

class ModuleManagerDlg : public QDialog
{
    Q_OBJECT

public:
    enum class ModuleType
    {
        RibbonModule,
        FuctionModule
    };

    struct ModuleInfo
    {
        QString name;
        QIcon icon;
        QString modulePath;
        ModuleType type{};
    };

    ModuleManagerDlg(const QList<ModuleInfo>& moduleInfoList, QSet<QString>& disabledModulePath, QWidget *parent = nullptr);
    ~ModuleManagerDlg();

protected:
    void AddTableRow(const QIcon& icon, const QString& name, const QString& path, bool isEnabled);

private slots:
    void OnDisabledComboIndexChanged(int index);

private:
    Ui::ModuleManagerDlgClass ui;
    QSet<QString>& m_disabledModulePath;
};
