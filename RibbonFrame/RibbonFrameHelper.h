#pragma once
#include <QMap>
#include "moduleinterface.h"
#include <QMenu>
#include <QAction>
#include <QTabWidget>
#include <QStackedWidget>
#include <QToolBar>
#include <QHBoxLayout>
#include <QLabel>
#include "settingsdialog.h"
#include "modulemanagerdlg.h"
#include <QDomElement>
#include "widgets/navigatewidget.h"
#include <QSplitter>
class QToolButton;

class RibbonFrameHelper
{
public:
    //获取xml节点中一个属性的bool值
    static bool GetAttributeBool(const QDomElement& element, const QString& attr, bool defaultVal = false);
    //根据一个xml节点判断对应的元素是否要以小图标的样式显示
    static bool IsSmallIcon(const QDomElement& element);
    //根据一个xml节点设置QToolButton的样式
    static void SetToolButtonStyle(QToolButton* pToolBtn, const QDomElement& element, Qt::ToolButtonStyle defaultStyle = Qt::ToolButtonTextUnderIcon);
    //获取一个xml元素的id
    static QString GetElementId(const QDomElement& element);
    //获取一个xml元素是否可用
    static bool GetElementEnabled(const QDomElement& element, bool defaultVal = true);
    //从路径创建一个指定大小的图标
    static QIcon CreateIcon(QString strPath, int size);

    static bool IsToolBarTag(const QString& tagName);

    static bool IsActionTag(const QString& tagName);
};

//保存主窗口的私有成员变量
class RibbonFramePrivate
{
public:
    QMap<int, IModule*> m_moduleIndexMap;       //保存标签索引和IModuleInterface对象的对应关系
    QMap<QString, IModule*> m_moduleNameMap;    //保存模块的名称和IModuleInterface对象的对应关系
    QMap<QString, IModule*> m_modulePathMap;    //保存已加载模块的路径和模块对象
    QMap<int, QString> m_moduleIndexPath;       //保存标签索引和模块路径的对应关系
    QMap<int, QMenu*> m_pageMenuMap;

    QMap<QString, QAction*> m_actionMap;    //保存命令Id和Action的对应关系
    QMap<QString, QWidget*> m_widgetMap;    //保存命令Id和Widget的对应关系
    QMap<QString, QMenu*> m_menuMap;        //保存命令Id和菜单的对应关系
    QMap<int, QStringList> m_cmdRadioBtnGroutMap;           //保存RadioButton或命令作为单选按钮的命令组

    QTabWidget* m_pTabWidget{};           //主窗口的TabWidget
    QStackedWidget* m_pStackedWidget{};   //切换不同模块的主窗口
    QToolBar* m_pTopRightBar{};           //TabWidget右上角的工具栏
    QHBoxLayout* m_pTopLeftLayout{};      //TabWidget左上角的布局，包含了系统按钮和快速启动栏

    NavigateWidget* m_pNavigateWidget{};    //显示在左侧的导航栏
    QSplitter* m_pNaviSplitter{};           //导航栏的分隔条
    QStackedWidget* m_pControlsStackedWidget{}; //切换不同模块的功能区（当显示了左侧导航栏时）
    QVBoxLayout* pNavigateLayout{};         //左侧的导航栏的布局

    QLabel* m_pNoMainWindowLabel;
    QLabel* m_pModuleLoadFailedLabel;

    QString m_windowTitle;
    QMap<WId, QWidget*> m_mfcWindowMap;  //保存已加载过的MFC窗口
    QWidget* m_pDefaultWidget{};
    QAction* actionPinRibbon{};
    QAction* actionRibbonOptions{};
    QAction* actionModuleManage{};

    QString m_xmlPath;
    bool m_inited{};
    bool ribbonShow{ true };    //功能区是否显示
    bool tabbarClicked{ false };    //点击ribbon标签后的500毫秒内为ture，其他时候为false
    bool showLeftNaviBar{};

    SettingsDialog::Data m_ribbonOptionData;

    QList<ModuleManagerDlg::ModuleInfo> moduleInfoList;
    QSet<QString> m_disabledModulePath;

public:
    RibbonFramePrivate(const QStringList& cmdLine);

    void LoadConfig();
    void SaveConfig() const;
    bool IsModuleManager() const;

private:
    bool m_isModuleManager = false;

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 左侧有图标的Label
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ImageLabel : public QWidget
{
public:
    ImageLabel(QWidget* pParent, const QString& iconPath, const QString& text = QString());
    void setIcon(const QIcon& icon);
    QLabel* TextLabel() const;

private:
    QLabel* pImageLabel;
    QLabel* pTextLabel;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CmdLineParser
{
public:
    CmdLineParser();
    bool IsModuleManager() const;

};
