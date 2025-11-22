#include "modulemanagerdlg.h"
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QListView>
#include "ribbonuipredefine.h"
#include "toolkits/moduletools.h"

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

    ui.treeWidget->setColumnCount(COL_MAX);
    ui.treeWidget->setHeaderLabels(QStringList{ u8"模块名称", u8"模块路径", u8"启用状态"});
    ui.treeWidget->setColumnWidth(COL_NAME, DPI(170));
    ui.treeWidget->setColumnWidth(COL_PATH, DPI(200));
    ui.treeWidget->setColumnWidth(COL_ENABLED, DPI(40));
    ui.treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //显示模块列表
    QMap<ModuleType, QTreeWidgetItem*> parentItemMap; //父节点map
    for (const auto& moduleInfo : moduleInfoList)
    {
        //创建父节点
        QTreeWidgetItem* parentItem = nullptr;
        auto iter = parentItemMap.find(moduleInfo.type);
        if (iter == parentItemMap.end())
        {
            QString strTypeName;
            switch (moduleInfo.type)
            {
            case ModuleManagerDlg::ModuleType::RibbonModule:
                strTypeName = u8"Ribbon模块";
                break;
            case ModuleManagerDlg::ModuleType::FuctionModule:
                strTypeName = u8"功能模块";
                break;
            default:
                strTypeName = u8"其他模块";
                break;
            }

            parentItem = AddTreeRow(nullptr, QIcon(), strTypeName, QString(), false);
            parentItemMap.insert(moduleInfo.type, parentItem);
        }
        else
        {
            parentItem = iter.value();
        }
        //添加到父节点下
        AddTreeRow(parentItem, moduleInfo.icon, moduleInfo.name, moduleInfo.modulePath, !disabledModulePath.contains(moduleInfo.modulePath));
    }

    ui.treeWidget->expandAll();
}

ModuleManagerDlg::~ModuleManagerDlg()
{}

//设置一个树节点整行的颜色
static void SetTreeItemColor(QTreeWidgetItem* treeItem, const QColor& color)
{
    for (int i = 0; i < COL_MAX; i++)
        treeItem->setBackground(i, color);
}

QTreeWidgetItem* ModuleManagerDlg::AddTreeRow(QTreeWidgetItem *parentItem, const QIcon &icon, const QString &name, const QString &path, bool isEnabled)
{
    QTreeWidgetItem* newItem = nullptr;
    //添加子节点
    if (parentItem != nullptr)
    {
        newItem = new QTreeWidgetItem(parentItem);
        parentItem->addChild(newItem);
    }
    //添加父节点
    else
    {
        newItem = new QTreeWidgetItem(ui.treeWidget);
        ui.treeWidget->addTopLevelItem(newItem);
        if (!CModuleTools::IsDarkTheme())   //仅浅色模式下设置背景色
        {
            SetTreeItemColor(newItem, QColor(229, 252, 194));
        }
    }
    newItem->setIcon(0, icon);
    newItem->setText(COL_NAME, name);
    newItem->setText(COL_PATH, path);
    if (!path.isEmpty())
    {
        QComboBox* pEnabledCombo = new QComboBox(ui.treeWidget);
        pEnabledCombo->setView(new QListView());
        pEnabledCombo->addItems(QStringList{ u8"启用", u8"禁用" });
        pEnabledCombo->setCurrentIndex(isEnabled ? 0 : 1);
        pEnabledCombo->setProperty("path", path);
        connect(pEnabledCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnDisabledComboIndexChanged(int)));
        ui.treeWidget->setItemWidget(newItem, COL_ENABLED, pEnabledCombo);
    }
    return newItem;
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
