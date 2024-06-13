#include "modulemanagerdlg.h"
#include <QTableWidgetItem>
#include <QComboBox>
#include <QListView>
#include "ribbonuipredefine.h"

enum Column
{
    COL_NAME,
    COL_PATH,
    COL_ENABLED,
    COL_MAX
};

ModuleManagerDlg::ModuleManagerDlg(const QList<ModuleInfo>& moduleInfoList, QSet<QString>& disabledModulePath, QWidget *parent)
    : QDialog(parent),
    m_disabledModulePath(disabledModulePath)
{
    ui.setupUi(this);
    if (layout() != nullptr)
        layout()->setMargin(DPI(8));
    resize(DPI(480), DPI(350));

    ui.tableWidget->setColumnCount(COL_MAX);
    ui.tableWidget->setHorizontalHeaderLabels(QStringList{ u8"模块名称", u8"模块路径", u8"启用状态"});
    ui.tableWidget->setColumnWidth(COL_NAME, DPI(140));
    ui.tableWidget->setColumnWidth(COL_PATH, DPI(170));
    ui.tableWidget->setColumnWidth(COL_ENABLED, DPI(100));
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //显示模块列表
    for (const auto& moduleInfo : moduleInfoList)
    {
        AddTableRow(moduleInfo.icon, moduleInfo.name, moduleInfo.modulePath, !disabledModulePath.contains(moduleInfo.modulePath));
    }
}

ModuleManagerDlg::~ModuleManagerDlg()
{}

void ModuleManagerDlg::AddTableRow(const QIcon & icon, const QString & name, const QString & path, bool isEnabled)
{
    int row = ui.tableWidget->rowCount();
    ui.tableWidget->setRowCount(row + 1);

    QTableWidgetItem* pNameItem = new QTableWidgetItem(icon, name);
    ui.tableWidget->setItem(row, COL_NAME, pNameItem);

    QTableWidgetItem* pPathItem = new QTableWidgetItem(path);
    ui.tableWidget->setItem(row, COL_PATH, pPathItem);

    QComboBox* pEnabledCombo = new QComboBox(ui.tableWidget);
    pEnabledCombo->setView(new QListView());
    pEnabledCombo->addItems(QStringList{ u8"启用", u8"禁用" });
    pEnabledCombo->setCurrentIndex(isEnabled ? 0 : 1);
    pEnabledCombo->setProperty("path", path);
    connect(pEnabledCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnDisabledComboIndexChanged(int)));
    ui.tableWidget->setCellWidget(row, COL_ENABLED, pEnabledCombo);
}

void ModuleManagerDlg::OnDisabledComboIndexChanged(int index)
{
    QString modulePath = sender()->property("path").toString();
    bool isEnabled = (index == 0);
    if (isEnabled)
        m_disabledModulePath.remove(modulePath);
    else
        m_disabledModulePath.insert(modulePath);
}