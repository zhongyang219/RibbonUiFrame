#include "ribbonframewindow.h"
#ifdef Q_OS_WIN
#include <qt_windows.h>
#pragma comment(lib, "User32.lib")
#include "toolkits/WinVersionHelper.h"
#endif
#include <QApplication>
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QDomDocument>
#include <QStatusBar>
#include <QMouseEvent>
#include <QLibrary>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QDomElement>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStackedWidget>
#include <QMenu>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTranslator>
#include <QWidgetAction>
#include <QListWidget>
#include <QWindow>
#include <QFileInfo>
#include <QActionGroup>
#include <QSplitter>
#include <QTimer>
#include <QSet>
#include "ribbonuipredefine.h"
#include "settingsdialog.h"
#include "modulemanagerdlg.h"
#include "RibbonFrameHelper.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RibbonFrameWindow::RibbonFrameWindow(QWidget *parent, const QString& xmlPath, bool initUiManual, const QStringList& cmdLine)
    : QMainWindow(parent)
{
    d = new RibbonFramePrivate(cmdLine);
    d->m_xmlPath = xmlPath;

    //设置主窗口最小大小
    setMinimumSize(DPI(400), DPI(300));

    //从xml文件获取应用程序名称
    RibbonFrameHelper::SetApplicationNameByXml(xmlPath);

    //载入功能区设置
    d->m_ribbonOptionData.Load();
    d->showLeftNaviBar = d->m_ribbonOptionData.showLeftNaviBar;

    //初始化主窗口
    QWidget* pCentralWidget = new QWidget(this);
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);
    pCentralWidget->setLayout(pLayout);
    setCentralWidget(pCentralWidget);
    //添加自定义标题栏
#ifdef Q_OS_WIN
    pLayout->addWidget(d->m_pTitleBar = new TitleBarWidget(this));
    installEventFilter(d->m_pTitleBar);
    d->m_pTitleBar->hide();
#endif

    if (d->showLeftNaviBar)
    {
        d->m_pNaviSplitter = new QSplitter(this);
        d->m_pNaviSplitter->setChildrenCollapsible(false);
        QWidget* pNavigateWidget = new QWidget();
        pNavigateWidget->setObjectName("MainFrameNavigationBar");
        d->pNavigateLayout = new QVBoxLayout();
        d->pNavigateLayout->setContentsMargins(0, 0, 0, 0);
        d->pNavigateLayout->setSpacing(0);
        pNavigateWidget->setLayout(d->pNavigateLayout);
        d->pNavigateLayout->addWidget(d->m_pNavigateWidget = new NavigateWidget(Qt::Vertical), 1);
        d->m_pNaviSplitter->addWidget(pNavigateWidget);
        QWidget* pRightWidget = new QWidget();
        d->m_pNaviSplitter->addWidget(pRightWidget);
        d->m_pNaviSplitter->setStretchFactor(0, 1);
        d->m_pNaviSplitter->setStretchFactor(1, 3);
        pLayout->addWidget(d->m_pNaviSplitter);
        QVBoxLayout* pRightLayout = new QVBoxLayout();
        pRightLayout->setContentsMargins(0, 0, 0, 0);
        pRightLayout->setSpacing(0);
        pRightWidget->setLayout(pRightLayout);
        pRightLayout->addWidget(d->m_pControlsStackedWidget = new QStackedWidget());
        pRightLayout->addWidget(d->m_pStackedWidget = new QStackedWidget(), 1);
        //为功能区添加一个右键菜单
        d->m_pNavigateWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    }
    else
    {
        //添加TabWidget
        pLayout->addWidget(d->m_pTabWidget = new QTabWidget());
        d->m_pTabWidget->setObjectName("MainFrameTab");
        d->m_pTabWidget->setAttribute(Qt::WA_StyledBackground);
        d->m_pTabWidget->tabBar()->setObjectName("MainFrameTabBar");
        d->m_pTabWidget->setStyleSheet(QString("QTabBar::tab{height:%1px}").arg(DPI(26)));     //设置tab标签的高度
        pLayout->addWidget(d->m_pStackedWidget = new QStackedWidget(), 1);
        //为功能区添加一个右键菜单
        d->m_pTabWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    }

    d->actionPinRibbon = AddRibbonContextAction(CMD_RibbonPin, QSTR("显示功能区"));
    d->actionPinRibbon->setCheckable(true);
    d->actionRibbonOptions = AddRibbonContextAction(CMD_RibbonOptions, QSTR("功能区设置..."));
    d->actionRibbonOptions->setIcon(QIcon(":/icon/res/settings.png"));
    d->actionModuleManage = AddRibbonContextAction(CMD_ModuleManager, QSTR("模块管理..."));
    d->actionModuleManage->setIcon(QIcon(":/icon/res/module.png"));

    //添加状态栏
    setStatusBar(new QStatusBar(this));

    //添加左上角控件
    QWidget* pTopLeftBar = new QWidget(this);
    if (d->showLeftNaviBar)
    {
        d->pNavigateLayout->insertWidget(0, pTopLeftBar);
    }
    else
    {
        d->m_pTabWidget->setCornerWidget(pTopLeftBar, Qt::TopLeftCorner);
    }
    d->m_pTopLeftLayout = new QHBoxLayout();
    d->m_pTopLeftLayout->setContentsMargins(0, 0, 0, 0);
    d->m_pTopLeftLayout->setSpacing(DPI(2));
    pTopLeftBar->setLayout(d->m_pTopLeftLayout);

    //在TabWidget右上角添加一个工具栏
    d->m_pTopRightBar = new QToolBar();
    d->m_pTopRightBar->setIconSize(QSize(ICON_SIZE_S, ICON_SIZE_S));
    d->m_pTopRightBar->setObjectName("MainFrameCornerBar");
    if (!d->showLeftNaviBar)
    {
        QWidget* pTopRightWidget = new QWidget();
        QHBoxLayout* pTopRightLayout = new QHBoxLayout();
        pTopRightLayout->setContentsMargins(0, 0, 0, 0);
        pTopRightWidget->setLayout(pTopRightLayout);
        pTopRightLayout->addWidget(d->m_pTopRightBar, 0, Qt::AlignVCenter);
        d->m_pTabWidget->setCornerWidget(pTopRightWidget);
    }

    if (!initUiManual)
        InitUi();
}

RibbonFrameWindow::~RibbonFrameWindow()
{
    //保存设置
    d->m_ribbonOptionData.Save();

    //保存禁用插件设置
    d->SaveConfig();

    for (auto iter = d->m_moduleNameMap.begin(); iter != d->m_moduleNameMap.end(); ++iter)
    {
        if (iter.value() != nullptr)
        {
            //反初始化插件
            iter.value()->UnInitInstance();
            //释放指针
            delete iter.value();
        }
    }
    delete d;
}

void RibbonFrameWindow::InitUi()
{
    if (!d->m_inited)
    {
        //加载功能模块
        LoadUIFromXml(d->m_xmlPath);

        //如果工具栏中有RadioButton，处理RadioButton的组
        ApplyRadioButtonGroup();

        //设置窗口标题
        QString strTitle = (d->m_windowTitle.isEmpty() ? qApp->applicationName() : d->m_windowTitle);
#ifdef QT_DEBUG
        //Debug模式下，在标题栏中显示“Debug模式”
        strTitle += QSTR(" (Debug 模式)");
#endif
        setWindowTitle(strTitle);

        //响应标签切换消息
        if (d->showLeftNaviBar)
        {
            connect(d->m_pNavigateWidget, SIGNAL(curItemChanged(int)), this, SLOT(OnTabIndexChanged(int)));
        }
        else
        {
            connect(d->m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(OnTabIndexChanged(int)));
            connect(d->m_pTabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(OnTabBarClicked(int)));
            connect(d->m_pTabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(OnTabBarDoubleClicked(int)));
        }

        QObject::connect(qApp, &QApplication::focusChanged, this, &RibbonFrameWindow::FocusChanged);

        //为“关于Qt”命令设置图标
        QIcon qtIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton);
        SetItemIcon("AppAboutQt", qtIcon);

        //设置“固定功能区”命令的选中状态
        d->actionPinRibbon->setChecked(d->m_ribbonOptionData.ribbonPin);
        d->actionPinRibbon->setEnabled(d->m_ribbonOptionData.ribbonHideEnable);
        SetItemChecked(CMD_RibbonPin, d->m_ribbonOptionData.ribbonPin);
        SetItemEnable(CMD_RibbonPin, d->m_ribbonOptionData.ribbonHideEnable);
        QTimer::singleShot(100, [this](){ SetRibbonPin(d->m_ribbonOptionData.ribbonPin); });

        d->m_inited = true;

        //如果命令行参数中包含模块管理器，则显示模块管理界面
        if (d->IsModuleManager())
        {
            ModuleManagerDlg dlg(d->moduleInfoList, d->m_disabledModulePath);
            dlg.exec();
            //退出程序
            QTimer::singleShot(0, qApp, &QCoreApplication::quit);
        }

        //设置无边框窗口
#ifdef Q_OS_WIN
        if (d->m_ribbonOptionData.customTitleBar)
        {
            HWND hWnd = (HWND)winId();
            LONG styles = GetWindowLong(hWnd, GWL_STYLE);
            //去掉Windows标题栏
            styles &= ~WS_CAPTION;
            SetWindowLong(hWnd, GWL_STYLE, styles);
            //显示自定义标题栏
            d->m_pTitleBar->show();
        }
#endif
    }
}


void RibbonFrameWindow::OnTabIndexChanged(int index)
{
    IModule* pModule = d->m_moduleIndexMap[index];
    QString modulePath = d->m_moduleIndexPath.value(index);
    if (pModule != nullptr)
    {
        pModule->OnTabEntered();
        QWidget* pModuleWidget = GetModuleMainWindow(pModule);
        if (pModuleWidget != nullptr)
            d->m_pStackedWidget->setCurrentWidget(pModuleWidget);
        else
            d->m_pStackedWidget->setCurrentWidget(d->m_pNoMainWindowLabel);
    }
    else if (!modulePath.isEmpty())
    {
        d->m_pStackedWidget->setCurrentWidget(d->m_pModuleLoadFailedLabel);
    }
    else if (d->m_pDefaultWidget != nullptr)
    {
        d->m_pStackedWidget->setCurrentWidget(d->m_pDefaultWidget);
    }
    if (d->showLeftNaviBar)
    {
        d->m_pControlsStackedWidget->setCurrentIndex(index);
    }

    ShowHideRibbon(d->m_ribbonOptionData.ribbonPin);
}

void RibbonFrameWindow::OnTabBarClicked(int index)
{
    if (d->m_ribbonOptionData.ribbonHideEnable && !d->showLeftNaviBar)
    {
        if (d->m_ribbonOptionData.showWhenTabClicked == SettingsDialog::Data::Ribbon)
        {
            d->tabbarClicked = true;
            QTimer::singleShot(500, [this](){ d->tabbarClicked = false; });
            Q_UNUSED(index)
            if (!d->m_ribbonOptionData.ribbonPin)
            {
                ShowHideRibbon(true);
            }
        }
        else if (d->m_ribbonOptionData.showWhenTabClicked == SettingsDialog::Data::Menu)
        {
            if (!d->m_ribbonOptionData.ribbonPin)
            {
                QMenu* pMenu = d->m_pageMenuMap[index];
                if (pMenu != nullptr)
                {
                    pMenu->move(d->m_pTabWidget->tabBar()->mapToGlobal(d->m_pTabWidget->tabBar()->tabRect(index).bottomLeft()));
                    pMenu->show();
                }
            }
        }
    }
}

void RibbonFrameWindow::OnTabBarDoubleClicked(int index)
{
    Q_UNUSED(index)

    if (d->m_ribbonOptionData.ribbonHideEnable && d->m_ribbonOptionData.ribbonDoubleClickEnable)
    {
        QTimer::singleShot(50, [this](){
            d->m_ribbonOptionData.ribbonPin = !d->m_ribbonOptionData.ribbonPin;
            SetItemChecked(CMD_RibbonPin, d->m_ribbonOptionData.ribbonPin);
            d->actionPinRibbon->setChecked(d->m_ribbonOptionData.ribbonPin);
            ShowHideRibbon(d->m_ribbonOptionData.ribbonPin);
        });
    }
}

void RibbonFrameWindow::OnActionTriggerd(bool checked)
{
    //所有模块的Action都在此处响应，QAction对象中的data保存了命令id
    QString strCmdId = QObject::sender()->property("id").toString();
    if (!strCmdId.isEmpty() && !OnCommand(strCmdId, checked))
    {
        //处理处于同一组RadioButton的命令的状态
        //查找命令所在组
        Q_FOREACH (const auto& cmdList, d->m_cmdRadioBtnGroutMap)
        {
            if (cmdList.contains(strCmdId))
            {
                checked = true;
                Q_FOREACH (const auto& strCmd, cmdList)
                {
                    SetItemChecked(strCmd.toUtf8().constData(), strCmd == strCmdId);
                }
                break;
            }
        }

        Q_FOREACH(const auto& module, d->m_moduleNameMap)
        {
            if (module != nullptr)
                module->OnCommand(strCmdId.toUtf8().constData(), checked);
        }
    }
}

void RibbonFrameWindow::OnItemIndexChanged(int index)
{
    QString strCmdId = QObject::sender()->property("id").toString();
    QString strText;
    QComboBox* pCombo = qobject_cast<QComboBox*>(QObject::sender());
    if (pCombo != nullptr)
    {
        strText = pCombo->itemText(index);
    }
    QListWidget* pListWidget = qobject_cast<QListWidget*>(QObject::sender());
    if (pListWidget != nullptr)
    {
        QListWidgetItem* pItem = pListWidget->item(index);
        if (pItem != nullptr)
            strText = pItem->text();
    }
    Q_FOREACH(const auto & module, d->m_moduleNameMap)
    {
        if (module != nullptr)
            module->OnItemChanged(strCmdId.toUtf8().constData(), index, strText.toUtf8().constData());
    }
    OnItemChanged(strCmdId, index, strText);
}

void RibbonFrameWindow::OnEditTextChanged(const QString& text)
{
    QString strCmdId = QObject::sender()->property("id").toString();
    Q_FOREACH(const auto & module, d->m_moduleNameMap)
    {
        if (module != nullptr)
            module->OnItemChanged(strCmdId.toUtf8().constData(), 0, text.toUtf8().constData());
    }
    OnItemChanged(strCmdId, 0, text);
}

void RibbonFrameWindow::OnEditTextChanged()
{
    QString strCmdId = QObject::sender()->property("id").toString();
    QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(QObject::sender());
    QString strText;
    if (pTextEdit != nullptr)
        strText = pTextEdit->toPlainText();
    Q_FOREACH(const auto & module, d->m_moduleNameMap)
    {
        if (module != nullptr)
            module->OnItemChanged(strCmdId.toUtf8().constData(), 0, strText.toUtf8().constData());
    }
    OnItemChanged(strCmdId, 0, strText);
}

void RibbonFrameWindow::FocusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)
    Q_UNUSED(now)
    if (d->m_ribbonOptionData.ribbonHideEnable && d->m_ribbonOptionData.showWhenTabClicked == SettingsDialog::Data::Ribbon)
    {
        if(!d->m_ribbonOptionData.ribbonPin && d->ribbonShow && !d->tabbarClicked)
        {
            ShowHideRibbon(false);
        }
    }
}

void RibbonFrameWindow::LoadUIFromXml(QString xmlPath)
{
    if (xmlPath.isEmpty())
        xmlPath = qApp->applicationDirPath() + "/MainFrame.xml";
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString strInfo = QSTR("打开 xml 文件“%1”失败！").arg(xmlPath);
        QMessageBox::critical(this, QString(), strInfo);
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.isNull())
    {
        QString strInfo = QSTR("MainFrame.xml文件找不到节点！");
        QMessageBox::critical(this, QString(), strInfo);
        return;
    }

    //设置字体
    QFont font = qApp->font();
    if (root.hasAttribute("font"))
        font.setFamily(root.attribute("font"));
#ifdef Q_OS_WIN
    //没有font属性时，Windows下默认设为微软雅黑字体
    else
        font.setFamily("Microsoft YaHei");
#endif
    int fontSize = root.attribute("fontSize").toInt();
    if (fontSize > 0)
        font.setPointSize(fontSize);
    qApp->setFont(font);

    //载入已禁用模块设置
    d->LoadConfig();

    //载入主框架命令
    LoadMainFrameUi(root);

    if (d->showLeftNaviBar)
    {
        d->m_pNavigateWidget->AddSpacing();
        d->pNavigateLayout->addWidget(d->m_pTopRightBar);
    }

    //显示隐藏状态栏
    QStatusBar* pStatusbar = statusBar();
    if (pStatusbar != nullptr)
        pStatusbar->setVisible(d->m_ribbonOptionData.showStatusBar);

    //通知插件UI加载完成
    Q_FOREACH(IModule* pModule, d->m_moduleNameMap)
    {
        if (pModule != nullptr)
            pModule->UiInitComplete(this);
    }
}

IModule *RibbonFrameWindow::LoadPlugin(const QString& strModulePath)
{
    //如果当前模块未加载，则载入模块
    IModule* pModule = d->m_modulePathMap[strModulePath];

    //载入模块
    if (!strModulePath.isEmpty() && pModule == nullptr)
    {
        QLibrary libMgr(strModulePath);
        if (libMgr.load())
        {
            //获取CreateInstance函数的入口地址
            pfCreateModuleInstance fun = (pfCreateModuleInstance)libMgr.resolve("CreateInstance");
            if (fun != nullptr)
            {
                pModule = fun();
                if (pModule != nullptr)
                {
                    pModule->InitInstance();        //初始化插件
                    d->m_moduleNameMap[pModule->GetModuleName()] = pModule;
                    d->m_modulePathMap[strModulePath] = pModule;
                }
                //保存主题模块接口
                IRibbonStyle* style = dynamic_cast<IRibbonStyle*>(pModule);
                if (style != nullptr)
                    d->m_ribbonStyle = style;
            }
            else
            {
                QString strInfo = QSTR("从模块“%1”中获取“CreateInstance”的入口地址失败！").arg(strModulePath);
                QMessageBox::critical(this, QString(), strInfo);
            }
        }
        else
        {
            QString strInfo = QSTR("加载插件%1失败!").arg(strModulePath);
            QMessageBox::critical(this, QString(), strInfo);
        }
    }
    return pModule;
}

void RibbonFrameWindow::LoadMainFrameUi(const QDomElement &element)
{
    //加载页面
    QDomNodeList childNode = element.childNodes();
    for(int i = 0; i < childNode.count(); i++)
    {
        QDomElement nodeInfo = childNode.at(i).toElement();
        QString tagName = nodeInfo.tagName();
        if (tagName == "Page")
        {
            QString strTabName = nodeInfo.attribute("name");
            QString strModulePath = nodeInfo.attribute("modulePath");

            ModuleManagerDlg::ModuleInfo moduleInfo;
            moduleInfo.type = ModuleManagerDlg::ModuleType::RibbonModule;
            moduleInfo.name = strTabName;
            moduleInfo.modulePath = strModulePath;

            //获取图标
            QString strIcon = nodeInfo.attribute("icon");
            QIcon tabIcon = RibbonFrameHelper::CreateIcon(strIcon, ICON_SIZE_S);
            moduleInfo.icon = tabIcon;
            if (!moduleInfo.modulePath.isEmpty())
                d->moduleInfoList.push_back(moduleInfo);

            //如果命令行参数中包含模块管理器，则不加载模块
            if (d->IsModuleManager())
                continue;

            //如果模块被禁用，则这里不再加载模块
            if (d->m_disabledModulePath.contains(strModulePath))
                continue;

            //载入模块
            IModule* pModule = LoadPlugin(strModulePath);
            int index;
            if (d->showLeftNaviBar)
                index = d->m_pNavigateWidget->GetItemCount();
            else
                index = d->m_pTabWidget->count();
            d->m_moduleIndexMap[index] = pModule;
            d->m_moduleIndexPath[index] = strModulePath;

            //添加标签页
            QToolBar* pToolbar = new QToolBar();
            if (d->showLeftNaviBar)
            {
                d->m_pNavigateWidget->AddItem(strTabName, tabIcon);
                d->m_pControlsStackedWidget->addWidget(pToolbar);
            }
            else
            {
                d->m_pTabWidget->addTab(pToolbar, tabIcon, strTabName);
            }
            pToolbar->setObjectName("MainFrameToolbar");
            //从模块获取主窗口
            if (pModule != nullptr)
            {
                QWidget* pModuleMainWindow = GetModuleMainWindow(pModule);
                if (pModuleMainWindow != nullptr)
                    d->m_pStackedWidget->addWidget(pModuleMainWindow);
                else
                    d->m_pStackedWidget->addWidget(d->m_pNoMainWindowLabel);
            }
            else
            {
                d->m_pStackedWidget->addWidget(d->m_pModuleLoadFailedLabel);
            }

            //加载命令组
            int elementCount = LoadUiElement(nodeInfo, pToolbar);
            d->m_ribbonEmpty[index] = (elementCount == 0);

            //将Page中所有控件加载到菜单
            QMenu* pPageMenu = LoadUiMenu(nodeInfo, false);
            d->m_pageMenuMap[index] = pPageMenu;
        }

        //加载主菜单
        else if (tagName == "SystemMenu")
        {
            QString menuName = nodeInfo.attribute("name");
            QString strIcon = nodeInfo.attribute("icon");

            //添加TabWidget左上角按钮
            QPushButton* pMainFrameBtn;
            if (d->showLeftNaviBar)
            {
                pMainFrameBtn = new QPushButton(menuName, d->m_pNavigateWidget);
                //设置按钮文本左对齐
                QString style = QString("QPushButton { text-align: left; padding-left: %1px; }").arg(DPI(6));
                pMainFrameBtn->setStyleSheet(style);
                d->pNavigateLayout->insertWidget(0, pMainFrameBtn);
            }
            else
            {
                pMainFrameBtn = new QPushButton(menuName, d->m_pTabWidget);
                d->m_pTopLeftLayout->addWidget(pMainFrameBtn, 0, Qt::AlignVCenter);
            }
            pMainFrameBtn->setObjectName("MainFrameBtn");
            if (!strIcon.isEmpty())
                pMainFrameBtn->setIcon(RibbonFrameHelper::CreateIcon(strIcon, ICON_SIZE_S));

            //为按钮添加菜单
            QMenu* pMainMenu = LoadUiMenu(nodeInfo);
            pMainMenu->setObjectName("MainFrameMenu");
            pMainFrameBtn->setMenu(pMainMenu);
            QString id = RibbonFrameHelper::GetElementId(nodeInfo);
            if (id.isEmpty())
                id = "SystemMenu";
            d->m_menuMap[id] = pMainMenu;
        }

        //加载快速启动栏
        else if (tagName == "QuickAccessBar")
        {
            QToolBar* pQuickAccessbar = new QToolBar(this);
            pQuickAccessbar->setObjectName("MainFrameCornerBar");
            pQuickAccessbar->setIconSize(QSize(ICON_SIZE_S, ICON_SIZE_S));
            if (d->m_ribbonOptionData.customTitleBar)
                d->m_pTitleBar->AddQuickAccessBar(pQuickAccessbar);
            else
                d->m_pTopLeftLayout->addWidget(pQuickAccessbar);
            LoadSimpleToolbar(nodeInfo, pQuickAccessbar);
            if (d->showLeftNaviBar)
                d->m_pTopLeftLayout->addStretch();
        }

        //加载状态栏
        else if (tagName == "StatusBar")
        {
            QStatusBar* pStatusbar = statusBar();
            if (pStatusbar == nullptr)
            {
                pStatusbar = new QStatusBar(this);
                setStatusBar(pStatusbar);
            }
            pStatusbar->setStyleSheet("margin-top:0px; margin-bottom:0px; padding-top:0px; padding-bottom:0px;");
            //加载状态栏左侧控件
            QToolBar* pLeftBar = new QToolBar(pStatusbar);
            pStatusbar->addWidget(pLeftBar);
            LoadSimpleToolbar(nodeInfo, pLeftBar);

            //加载状态栏右侧控件
            QToolBar* pRightBar = new QToolBar(pStatusbar);
            pStatusbar->addPermanentWidget(pRightBar);
            QDomNodeList statusbarElements = nodeInfo.childNodes();
            for(int i = 0; i < statusbarElements.count(); i++)
            {
                QDomElement statusbarElement = statusbarElements.at(i).toElement();
                if (statusbarElement.tagName() == "PermanentWidget")
                {
                    LoadSimpleToolbar(statusbarElement, pRightBar);
                }
            }
        }

        //找到MainWindow节点时递归调用此函数
        else if (tagName == "MainWindow")
        {
            d->m_windowTitle = nodeInfo.attribute("title");
            LoadMainFrameUi(nodeInfo);
        }

        //加载其他插件
        else if (tagName == "Plugins")
        {
            QDomNodeList pluginElements = nodeInfo.childNodes();
            for(int i = 0; i < pluginElements.count(); i++)
            {
                QDomElement pluginElement = pluginElements.at(i).toElement();
                QString modulePath = pluginElement.attribute("path");

                ModuleManagerDlg::ModuleInfo moduleInfo;
                moduleInfo.type = ModuleManagerDlg::ModuleType::FuctionModule;
                moduleInfo.name = QFileInfo(modulePath).baseName();
                moduleInfo.modulePath = modulePath;

                //获取图标
                moduleInfo.icon = RibbonFrameHelper::CreateIcon(":/icon/res/plugin.png", ICON_SIZE_S);
                if (!moduleInfo.modulePath.isEmpty())
                    d->moduleInfoList.push_back(moduleInfo);

                //如果命令行参数中包含模块管理器，则不加载模块
                if (d->IsModuleManager())
                    continue;

                //如果模块被禁用，则这里不再加载模块
                if (d->m_disabledModulePath.contains(modulePath))
                    continue;

                LoadPlugin(modulePath);
            }
        }

    }
    //加载右上角命令
    LoadSimpleToolbar(element, d->m_pTopRightBar);
}

int RibbonFrameWindow::LoadUiElement(const QDomElement &emelemt, QToolBar* pToolbar)
{
    int elementCount = 0;
    QVBoxLayout* previousLayout = nullptr;  //垂直排列小图标的布局，如果为空，则需要重新创建一个新的布局
    QDomNodeList groupList = emelemt.childNodes();
    for (int i = 0; i < groupList.count(); i++)
    {
        QDomElement childElement = groupList.at(i).toElement();
        QString strTagName = childElement.tagName();
        QString strName = childElement.attribute("name");
        QString strId = RibbonFrameHelper::GetElementId(childElement);
        if (!strTagName.isEmpty())
            elementCount++;
        //分隔符
        if (strTagName == "Separator")
        {
            pToolbar->addSeparator();
            previousLayout = nullptr;
        }
        //菜单按钮
        else if (strTagName == "Menu")
        {
            //获取菜单名称和图标
            bool smallIcon = RibbonFrameHelper::IsSmallIcon(childElement);
            //创建工具栏按钮
            QToolButton* pToolButton = new QToolButton(pToolbar);
            AddUiWidget(pToolButton, smallIcon, pToolbar, previousLayout);
            RibbonFrameHelper::SetToolButtonStyle(pToolButton, childElement);
            InitMenuButton(pToolButton, childElement);
            QString menuIconPath = childElement.attribute("icon");
            pToolButton->setIcon(RibbonFrameHelper::CreateIcon(menuIconPath, smallIcon ? ICON_SIZE_S : ICON_SIZE));

            //创建菜单并添加到工具栏按钮中
            QMenu* pMenu = LoadUiMenu(childElement);
            pToolButton->setMenu(pMenu);
            d->m_widgetMap[strId] = pToolButton;
        }
        //ActionGroup
        else if (strTagName == "ActionGroup")
        {
            previousLayout = nullptr;

            if (i > 0)
            {
                QString previousTagName = groupList.at(i - 1).toElement().tagName();
                //如果前一个元素没有添加分隔符，则在这里添加一个分隔符
                if (previousTagName != "ActionGroup")
                    pToolbar->addSeparator();
            }

            QToolBar* pSubToolbar = new QToolBar;
            pSubToolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
            if (strName.isEmpty())
            {
                pToolbar->addWidget(pSubToolbar);
            }
            else
            {
                QWidget* pActionGroupWidget = new QWidget();
                pActionGroupWidget->setObjectName("MainFrameActionGroup");
                pToolbar->addWidget(pActionGroupWidget);
                QVBoxLayout* pActionGroupLayout = new QVBoxLayout;
                pActionGroupWidget->setLayout(pActionGroupLayout);
                pActionGroupLayout->setContentsMargins(0, 0, 0, 0);
                pActionGroupLayout->setSpacing(0);
                pActionGroupLayout->addWidget(pSubToolbar, 1);
                //添加名称标签
                QHBoxLayout* pLabelLayout = new QHBoxLayout;
                pLabelLayout->setContentsMargins(0, 0, 0, 0);
                pLabelLayout->setSpacing(0);
                pActionGroupLayout->addLayout(pLabelLayout);
                QLabel* pGroupNameLabel = new QLabel(strName);
                pGroupNameLabel->setObjectName("ActionGroupNameLabel");
                pGroupNameLabel->setAlignment(Qt::AlignCenter);
                pLabelLayout->addWidget(pGroupNameLabel);
                //添加选项按钮
                bool hasOptionBtn = RibbonFrameHelper::GetAttributeBool(childElement, "optionBtn");
                if (hasOptionBtn)
                {
                    QToolButton* pOptionBtn = new QToolButton(pSubToolbar);
                    pLabelLayout->addWidget(pOptionBtn);
                    pOptionBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
                    pOptionBtn->setFixedSize(ACTION_GROUP_OPTION_BTN_SIZE, ACTION_GROUP_OPTION_BTN_SIZE);
                    QString strId = RibbonFrameHelper::GetElementId(childElement);
                    QAction* pOptionAction = LoadUiAction(childElement);
                    pOptionAction->setIcon(RibbonFrameHelper::CreateIcon(":/icon/res/ribbonOptionBtn.png", ACTION_GROUP_OPTION_ICON_SIZE));
                    pOptionBtn->setDefaultAction(pOptionAction);
                    d->m_widgetMap[strId] = pOptionBtn;
                }
            }

            elementCount += LoadUiElement(childElement, pSubToolbar);  //递归调用此函数以加载ActionGroup下的其他界面元素

            //则添加分隔符
            pToolbar->addSeparator();
        }
        else if (RibbonFrameHelper::IsToolBarTag(strTagName))
        {
            QToolBar* pSubToolbar = new QToolBar;
            LoadSimpleToolbar(childElement, pSubToolbar);
            AddUiWidget(pSubToolbar, RibbonFrameHelper::IsSmallIcon(childElement), pToolbar, previousLayout);
        }
        else if (RibbonFrameHelper::IsActionTag(strTagName))
        {
            bool smallItem = RibbonFrameHelper::IsSmallIcon(childElement);
            QAction* pAction = LoadUiAction(childElement);
            QToolButton* pToolButton = new QToolButton(pToolbar);
            AddUiWidget(pToolButton, smallItem, pToolbar, previousLayout);
            RibbonFrameHelper::SetToolButtonStyle(pToolButton, childElement);
            pToolButton->setDefaultAction(pAction);
            d->m_widgetMap[strId] = pToolButton;
        }
        else
        {
            bool smallIcon = RibbonFrameHelper::IsSmallIcon(childElement);
            QWidget* pUiWidget = LoadUiWidget(childElement, pToolbar, smallIcon);
            if (pUiWidget != nullptr)
                AddUiWidget(pUiWidget, smallIcon, pToolbar, previousLayout);
        }
    }
    return elementCount;
}

void RibbonFrameWindow::LoadSimpleToolbar(const QDomElement &element, QToolBar *pToolbar)
{
    pToolbar->setIconSize(QSize(ICON_SIZE_S, ICON_SIZE_S));
    pToolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    QDomNodeList groupList = element.childNodes();
    for (int i = 0; i < groupList.count(); i++)
    {
        QDomElement childElement = groupList.at(i).toElement();
        QString strTagName = childElement.tagName();
        if (RibbonFrameHelper::IsActionTag(strTagName))
        {
            QAction* pAction = LoadUiAction(childElement);
            QToolButton* pToolButton = new QToolButton(pToolbar);
            RibbonFrameHelper::SetToolButtonStyle(pToolButton, childElement, Qt::ToolButtonIconOnly);
            pToolButton->setDefaultAction(pAction);
            pToolbar->addWidget(pToolButton);
            QString strId = RibbonFrameHelper::GetElementId(childElement);
            d->m_widgetMap[strId] = pToolButton;
        }
        else if (strTagName == "Separator")
        {
            pToolbar->addSeparator();
        }
        //菜单按钮
        else if (strTagName == "Menu")
        {
            //创建工具栏按钮
            QToolButton* pToolButton = new QToolButton(pToolbar);
            pToolbar->addWidget(pToolButton);
            RibbonFrameHelper::SetToolButtonStyle(pToolButton, childElement, Qt::ToolButtonIconOnly);
            InitMenuButton(pToolButton, childElement);
            QString menuIconPath = childElement.attribute("icon");
            pToolButton->setIcon(RibbonFrameHelper::CreateIcon(menuIconPath, ICON_SIZE_S));

            //创建菜单并添加到工具栏按钮中
            QMenu* pMenu = LoadUiMenu(childElement);
            pToolButton->setMenu(pMenu);
            QString strId = RibbonFrameHelper::GetElementId(childElement);
            d->m_widgetMap[strId] = pToolButton;
        }
        else
        {
            bool smallIcon = true;
            QWidget* pWidget = LoadUiWidget(childElement, pToolbar, smallIcon);
            if (pWidget != nullptr)
                pToolbar->addWidget(pWidget);
        }
    }
}

QAction *RibbonFrameWindow::LoadUiAction(const QDomElement &actionNodeInfo)
{
    QString strCmdId = RibbonFrameHelper::GetElementId(actionNodeInfo);
    if (d->m_actionMap.contains(strCmdId))
    {
        return d->m_actionMap.value(strCmdId);
    }
    QString strCmdName = actionNodeInfo.attribute("name");
    QString strIconPath = actionNodeInfo.attribute("icon");
    bool bCheckable = RibbonFrameHelper::GetAttributeBool(actionNodeInfo,"checkable");
    bool checked = RibbonFrameHelper::GetAttributeBool(actionNodeInfo,"checked");
    QString strRadioGroup = actionNodeInfo.attribute("radioGroup");
    QString strShortcut = actionNodeInfo.attribute("shortcut");
    bool smallIcon = RibbonFrameHelper::IsSmallIcon(actionNodeInfo);
    if (!strRadioGroup.isEmpty())
    {
        bCheckable = true;
        d->m_cmdRadioBtnGroutMap[strRadioGroup.toInt()].push_back(strCmdId);
    }
    QString strTip = actionNodeInfo.attribute("tip");

    QAction* pAction = new QAction(RibbonFrameHelper::CreateIcon(strIconPath, smallIcon ? ICON_SIZE_S : ICON_SIZE), strCmdName, this);
    pAction->setProperty("id", strCmdId);       //将命令的ID作为用户数据保存到QAction对象中
    pAction->setCheckable(bCheckable);
    pAction->setChecked(checked);
    pAction->setEnabled(RibbonFrameHelper::GetElementEnabled(actionNodeInfo));
    pAction->setToolTip(strTip);
    pAction->setShortcut(QKeySequence(strShortcut));

    if (!strCmdId.isEmpty())
        d->m_actionMap[strCmdId] = pAction;
    connect(pAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTriggerd(bool)));
    return pAction;
}

QWidget *RibbonFrameWindow::LoadUiWidget(const QDomElement &element, QWidget *pParent, bool &smallIcon)
{
    QString strTagName = element.tagName();
    QString strName = element.attribute("name");
    QString strId = RibbonFrameHelper::GetElementId(element);
    QWidget* pUiWidget = nullptr;
    if (!strId.isEmpty() && d->m_widgetMap.contains(strId))
    {
        return d->m_widgetMap.value(strId);
    }

    if (strTagName == "Label")
    {
        QString strIcon = element.attribute("icon");
        smallIcon = true;
        //没有图标，只显示一个QLabel
        if (strIcon.isEmpty())
        {
            QLabel* pLabel = new QLabel(pParent);
            pLabel->setText(strName);
            pUiWidget = pLabel;
        }
        //有图标，使用ImageLabel
        else
        {
            pUiWidget = new ImageLabel(pParent, strIcon, strName);
        }
    }
    else if (strTagName == "LineEdit")
    {
        QLineEdit* pLineEdit = new QLineEdit(pParent);
        smallIcon = true;
        pLineEdit->setText(strName);
        pLineEdit->setMaximumWidth(DPI(120));
        pLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        bool editable = RibbonFrameHelper::GetAttributeBool(element, "editable", true);
        pLineEdit->setReadOnly(!editable);
        connect(pLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnEditTextChanged(const QString&)));
        pUiWidget = pLineEdit;
    }
    else if (strTagName == "TextEdit" || strTagName == "Edit")
    {
        QTextEdit* pTextEdit = new QTextEdit(pParent);
        smallIcon = false;
        pTextEdit->setPlainText(strName);
        pTextEdit->setMaximumWidth(DPI(120));
        pTextEdit->setMaximumHeight(MAX_WIDGET_HEIGHT);
        bool editable = RibbonFrameHelper::GetAttributeBool(element, "editable", true);
        pTextEdit->setReadOnly(!editable);
        connect(pTextEdit, SIGNAL(textChanged()), this, SLOT(OnEditTextChanged()));
        pUiWidget = pTextEdit;
    }
    else if (strTagName == "ComboBox")
    {
        QComboBox* pComboBox = new QComboBox(pParent);
        pComboBox->setView(new QListView);
        smallIcon = true;
        bool editable = RibbonFrameHelper::GetAttributeBool(element, "editable");
        pComboBox->setEditable(editable);
        pComboBox->setEditText(strName);
        pUiWidget = pComboBox;
        //加载下拉列表项
        QDomNodeList itemElements = element.childNodes();
        for (int i = 0; i < itemElements.count(); i++)
        {
            QDomElement itemElement = itemElements.at(i).toElement();
            QString itemText = itemElement.attribute("name");
            QString iconPath = itemElement.attribute("icon");
            pComboBox->addItem(RibbonFrameHelper::CreateIcon(iconPath, ICON_SIZE_S), itemText);
        }
        connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnItemIndexChanged(int)));
        connect(pComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(OnEditTextChanged(const QString&)));
    }
    else if (strTagName == "CheckBox")
    {
        QCheckBox* pCheckBox = new QCheckBox(pParent);
        smallIcon = true;
        pCheckBox->setText(strName);
        pCheckBox->setProperty("id", strId);
        bool checked = RibbonFrameHelper::GetAttributeBool(element,"checked");
        pCheckBox->setChecked(checked);
        connect(pCheckBox, SIGNAL(clicked(bool)), this, SLOT(OnActionTriggerd(bool)));
        pUiWidget = pCheckBox;
    }
    else if (strTagName == "RadioButton")
    {
        QRadioButton* pRadioButton = new QRadioButton(pParent);
        smallIcon = true;
        pRadioButton->setText(strName);
        QString strRadioGroup = element.attribute("radioGroup");
        if (!strRadioGroup.isEmpty())
        {
            d->m_cmdRadioBtnGroutMap[strRadioGroup.toInt()].push_back(strId);
        }
        pRadioButton->setProperty("id", strId);
        bool checked = RibbonFrameHelper::GetAttributeBool(element,"checked");
        pRadioButton->setChecked(checked);
        connect(pRadioButton, SIGNAL(clicked(bool)), this, SLOT(OnActionTriggerd(bool)));
        pUiWidget = pRadioButton;
    }
    else if (strTagName == "ListWidget")
    {
        QListWidget* pListWidget = new QListWidget(pParent);
        smallIcon = false;
        bool horizontalArrange = RibbonFrameHelper::GetAttributeBool(element, "horizontalArrange");
        if (horizontalArrange)
            pListWidget->setFlow(QListView::LeftToRight);
        pListWidget->setViewMode(QListView::ListMode);
        pListWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        pListWidget->setMaximumHeight(MAX_WIDGET_HEIGHT);
        pUiWidget = pListWidget;
        //加载列表项
        QDomNodeList itemElements = element.childNodes();
        for (int i = 0; i < itemElements.count(); i++)
        {
            QDomElement itemElement = itemElements.at(i).toElement();
            QString itemText = itemElement.attribute("name");
            QString iconPath = itemElement.attribute("icon");
            pListWidget->addItem(new QListWidgetItem(RibbonFrameHelper::CreateIcon(iconPath, ICON_SIZE_S), itemText));
        }
        connect(pListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(OnItemIndexChanged(int)));
    }
    else if (strTagName == "UserWidget")
    {
        QWidget* pUserWidget = nullptr;
        Q_FOREACH(IModule* pModule, d->m_moduleNameMap)
        {
            if (pModule->GetMainWindowType() == IModule::MT_QWIDGET)
            {
                pUserWidget = (QWidget*)pModule->CreateUserWidget(strId.toUtf8().constData(), pParent);
                if (pUserWidget != nullptr)
                    break;
            }
        }
        if (pUserWidget == nullptr)
            pUserWidget = CreateUserWidget(strId, pParent);
        if (pUserWidget == nullptr)
            pUserWidget = new QLabel(strName.isEmpty() ? "UserWidget" : strName);
        pUserWidget->setMaximumHeight(MAX_WIDGET_HEIGHT);
        smallIcon = RibbonFrameHelper::IsSmallIcon(element);
        pUiWidget = pUserWidget;
    }
    else if (strTagName == "WidgetGroup")
    {
        bool horizontal = RibbonFrameHelper::GetAttributeBool(element, "horizontalArrange", true);
        QLayout* pLayout = nullptr;
        if (horizontal)
            pLayout = new QHBoxLayout();
        else
            pLayout = new QVBoxLayout();
        pLayout->setContentsMargins(0, 0, 0, 0);
        pUiWidget = new QWidget(pParent);
        pUiWidget->setLayout(pLayout);
        //遍历子节点
        QDomNodeList childNodeList = element.childNodes();
        for (int i = 0; i < childNodeList.count(); i++)
        {
            QDomElement childElement = childNodeList.at(i).toElement();
            QString childTagName = childElement.tagName();
            bool childSmallIcon;
            if (RibbonFrameHelper::IsToolBarTag(childTagName))
            {
                QToolBar* pChildToolbar = new QToolBar(pUiWidget);
                LoadSimpleToolbar(childElement, pChildToolbar);
                pLayout->addWidget(pChildToolbar);
            }
            else
            {
                QWidget* pChildWidget = LoadUiWidget(childElement, pUiWidget, childSmallIcon);
                if (pChildWidget != nullptr)
                    pLayout->addWidget(pChildWidget);
            }
        }
        smallIcon = RibbonFrameHelper::IsSmallIcon(element);
}
    if (!strId.isEmpty())
        d->m_widgetMap[strId] = pUiWidget;
    if (pUiWidget != nullptr)
    {
        pUiWidget->setProperty("id", strId);
        QString strTip = element.attribute("tip");
        if (!strTip.isEmpty())
            pUiWidget->setToolTip(strTip);
        int width = element.attribute("width").toInt();
        int height = element.attribute("height").toInt();
        if (width > 0)
            pUiWidget->setFixedWidth(DPI(width));
        if (height > 0)
            pUiWidget->setFixedHeight(DPI(height));
        pUiWidget->setEnabled(RibbonFrameHelper::GetElementEnabled(element));
    }
    return pUiWidget;
}

QMenu *RibbonFrameWindow::LoadUiMenu(const QDomElement &element, bool enableWidget)
{
    //创建菜单
    QString strCmdName = element.attribute("name");
    QString strIconPath = element.attribute("icon");
    QMenu* pMenu = new QMenu(strCmdName);
    QString strId = RibbonFrameHelper::GetElementId(element);
    if (!strId.isEmpty())
        d->m_menuMap[strId] = pMenu;
    pMenu->setIcon(RibbonFrameHelper::CreateIcon(strIconPath, ICON_SIZE_S));
#if (QT_VERSION >= QT_VERSION_CHECK(5,1,0))
    pMenu->setToolTipsVisible(true);
#endif
    LoadMenuItemFromXml(element, pMenu, enableWidget);
    return pMenu;
}

void RibbonFrameWindow::LoadMenuItemFromXml(const QDomElement &element, QMenu *pMenu, bool enableWidget)
{
    //为菜单添加Action
    QDomNodeList menuList = element.childNodes();
    for (int k = 0; k < menuList.count(); k++)
    {
        QDomElement menuNodeInfo = menuList.at(k).toElement();
        QString strTagName = menuNodeInfo.tagName();
        //命令
        if (RibbonFrameHelper::IsActionTag(strTagName))
        {
            QAction* pAction = LoadUiAction(menuNodeInfo);
            pMenu->addAction(pAction);
        }
        //分隔符
        else if (strTagName == "Separator")
        {
            pMenu->addSeparator();
        }
        //子菜单
        else if (strTagName == "Menu")
        {
            QMenu* pSubMenu = LoadUiMenu(menuNodeInfo);     //递归添加子菜单
            pMenu->addMenu(pSubMenu);
        }
        //工具栏
        else if (RibbonFrameHelper::IsToolBarTag(strTagName))
        {
            QToolBar* pToolbar = new QToolBar(pMenu);
            pToolbar->setIconSize(QSize(ICON_SIZE_S, ICON_SIZE_S));
            LoadSimpleToolbar(menuNodeInfo, pToolbar);
            QWidgetAction* pWidgetAction = new QWidgetAction(this);
            pWidgetAction->setDefaultWidget(pToolbar);
            pMenu->addAction(pWidgetAction);
        }
        else if (strTagName == "ActionGroup"  || strTagName == "WidgetGroup")
        {
            auto actions = pMenu->actions();
            if (!actions.isEmpty() && !actions.back()->isSeparator())
                pMenu->addSeparator();

            QString strId = RibbonFrameHelper::GetElementId(menuNodeInfo);
            QString strName = menuNodeInfo.attribute("name");
            if (!strId.isEmpty())
            {
                QAction* pGroupAction = LoadUiAction(menuNodeInfo);
                pMenu->addAction(pGroupAction);
            }
            else if (!strName.isEmpty())
            {
                QWidgetAction* pWidgetAction = new QWidgetAction(pMenu);
                pWidgetAction->setDefaultWidget(new QLabel(strName));
                pMenu->addAction(pWidgetAction);
            }
            LoadMenuItemFromXml(menuNodeInfo, pMenu, enableWidget);
        }
        //其他控件
        else if (enableWidget)
        {
            bool smallIcon;
            QWidget* pUiWidget = LoadUiWidget(menuNodeInfo, pMenu, smallIcon);
            if (pUiWidget != nullptr)
            {
                QWidgetAction* pWidgetAction = new QWidgetAction(pMenu);
                pWidgetAction->setDefaultWidget(pUiWidget);
                pMenu->addAction(pWidgetAction);
            }
        }
    }
}

void RibbonFrameWindow::InitMenuButton(QToolButton *pMenuBtn, const QDomElement &element)
{
    //添加Action
    bool menuBtn = RibbonFrameHelper::GetAttributeBool(element, "menuBtn");
    if (menuBtn)
    {
        QAction* pAction = LoadUiAction(element);
        pMenuBtn->setDefaultAction(pAction);
        pMenuBtn->setPopupMode(QToolButton::MenuButtonPopup);
    }
    else
    {
        pMenuBtn->setPopupMode(QToolButton::InstantPopup);
    }
    QString strName = element.attribute("name");
    pMenuBtn->setText(strName);
    QString strTip = element.attribute("tip");
    if (pMenuBtn->toolButtonStyle() == Qt::ToolButtonIconOnly || !strTip.isEmpty())
        pMenuBtn->setToolTip(strTip.isEmpty() ? strName : strTip);
}


void RibbonFrameWindow::AddUiWidget(QWidget* pUiWidget, bool smallIcon, QToolBar *pToolbar, QVBoxLayout *&previousLayout)
{
    //不是小图标时直接将Widget添加到工具栏
    if (!smallIcon)
    {
        pToolbar->addWidget(pUiWidget);
        previousLayout = nullptr;
    }
    //是小图标时则需要将若干个小图标垂直排列
    else
    {
        //布局为空，则需要重新创建一个新的布局
        if (previousLayout == nullptr)
        {
            QWidget* pWidget = new QWidget;
            pWidget->setLayout(previousLayout = new QVBoxLayout);
            previousLayout->setContentsMargins(DPI(2), DPI(2), DPI(2), DPI(2));
            previousLayout->setSpacing(DPI(2));
            pToolbar->addWidget(pWidget);
        }
        //将Widget添加到布局中
        previousLayout->addWidget(pUiWidget);
        //如果一个布局中Widget的数量超过指定值，则将previousLayout置为空，下次将重新创建一个新的布局。
        if (previousLayout->count() >= MAX_SMALL_ICON_COLUMN)
            previousLayout = nullptr;
    }
}


IModule* RibbonFrameWindow::CurrentModule() const
{
    int curIndex;
    if (d->showLeftNaviBar)
        curIndex = d->m_pNavigateWidget->GetCurItem();
    else
        curIndex = d->m_pTabWidget->currentIndex();
    return d->m_moduleIndexMap.value(curIndex);
}

bool RibbonFrameWindow::OnCommand(const QString &strCmd, bool checked)
{
    Q_UNUSED(checked)
    if (strCmd == CMD_AppExit)
    {
        close();
        return true;
    }
    else if (strCmd == CMD_AppAboutQt)
    {
        QMessageBox::aboutQt(this);
        return true;
    }
    else if (strCmd == CMD_RibbonPin)
    {
        SetRibbonPin(checked);
        return true;
    }
    else if (strCmd == CMD_RibbonOptions)
    {
        SettingsDialog dlg(d->m_ribbonStyle, this);
        dlg.SetData(d->m_ribbonOptionData);
        if (dlg.exec() == QDialog::Accepted)
        {
            d->m_ribbonOptionData = dlg.GetData();
            SetItemChecked(CMD_RibbonPin, d->m_ribbonOptionData.ribbonPin);
            d->actionPinRibbon->setChecked(d->m_ribbonOptionData.ribbonPin);
            d->actionPinRibbon->setEnabled(d->m_ribbonOptionData.ribbonHideEnable);
            SetItemChecked(CMD_RibbonPin, d->m_ribbonOptionData.ribbonPin);
            SetItemEnable(CMD_RibbonPin, d->m_ribbonOptionData.ribbonHideEnable);
            ShowHideRibbon(d->m_ribbonOptionData.ribbonPin);
            QStatusBar* pStatusbar = statusBar();
            if (pStatusbar != nullptr)
                pStatusbar->setVisible(d->m_ribbonOptionData.showStatusBar);
        }
    }
    else if (strCmd == CMD_ModuleManager)
    {
        ModuleManagerDlg dlg(d->moduleInfoList, d->m_disabledModulePath);
        dlg.exec();
    }
    return false;
}

void RibbonFrameWindow::ApplyRadioButtonGroup()
{
    for (auto iter = d->m_cmdRadioBtnGroutMap.begin(); iter != d->m_cmdRadioBtnGroutMap.end(); ++iter)
    {
        //为所有具有相同命令组的RadioButton对象创建按钮组。
        QActionGroup* actionGroup = nullptr;
        QButtonGroup* btnGroup = nullptr;
        Q_FOREACH(const QString& id, iter.value())
        {
            QAction* pAction = _GetAction(id.toUtf8().constData());
            if (pAction != nullptr)
            {
                if (actionGroup == nullptr)
                    actionGroup = new QActionGroup(this);
                actionGroup->addAction(pAction);
                continue;
            }

            QRadioButton* pRadioBtn = qobject_cast<QRadioButton*>(_GetWidget(id.toUtf8().constData()));
            if (pRadioBtn != nullptr)
            {
                if (btnGroup == nullptr)
                    btnGroup = new QButtonGroup(this);
                btnGroup->addButton(pRadioBtn);
            }
        }
    }
}

QWidget *RibbonFrameWindow::GetModuleMainWindow(IModule *pModule)
{
    QWidget* pWidget{};
    IModule::eMainWindowType windowType = pModule->GetMainWindowType();
    if (windowType == IModule::MT_HWND)
    {
        WId wid = (WId)(pModule->GetMainWindow());
        if (wid != 0)
        {
            pWidget = d->m_mfcWindowMap[wid];
            if (pWidget == nullptr)
            {
                pWidget = QWidget::createWindowContainer(QWindow::fromWinId(wid));
                d->m_mfcWindowMap[wid] = pWidget;
            }
        }
    }
    else
    {
        pWidget = (QWidget*)pModule->GetMainWindow();
    }
    return pWidget;
}

void RibbonFrameWindow::SetRibbonPin(bool pin)
{
    d->m_ribbonOptionData.ribbonPin = pin;
    SetItemChecked(CMD_RibbonPin, pin);
    d->actionPinRibbon->setChecked(pin);
    ShowHideRibbon(pin);
}

void RibbonFrameWindow::ShowHideRibbon(bool show)
{
    d->ribbonShow = show;
    int curIndex = GetTabIndex();
    bool ribbonEmpty = d->m_ribbonEmpty[curIndex];
    //如果Ribbon为空，则总是不显示标签
    if (ribbonEmpty)
        show = false;

    if (d->showLeftNaviBar)
    {
        d->m_pControlsStackedWidget->setVisible(show);
    }
    else
    {
        if (show)
        {
            d->m_pTabWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        }
        else
        {
            d->m_pTabWidget->setMaximumHeight(d->m_pTabWidget->tabBar()->height() + DPI(1));
        }
    }
}

QAction *RibbonFrameWindow::AddRibbonContextAction(const QString &strId, const QString &strName)
{
    QAction* pAction = new QAction(strName);
    pAction->setProperty("id", strId);
    connect(pAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTriggerd(bool)));
    if (d->showLeftNaviBar)
        d->m_pNavigateWidget->addAction(pAction);
    else
        d->m_pTabWidget->addAction(pAction);
    return pAction;
}

void RibbonFrameWindow::closeEvent(QCloseEvent* event)
{
    for (auto iter = d->m_moduleNameMap.begin(); iter != d->m_moduleNameMap.end(); ++iter)
    {
        IModule* pModule = iter.value();
        if (pModule != nullptr)
        {
            pModule->OnAppExit();
        }
    }
    QMainWindow::closeEvent(event);
}

bool RibbonFrameWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG *msg = static_cast<MSG*>(message);
        switch(msg->message)
        {
        case WM_NCCALCSIZE:
        {
            if (CWinVersionHelper::IsWindows10OrLater() && d->m_ribbonOptionData.customTitleBar && !isMaximized())
            {
                BOOL bCalcValidRects = (BOOL)msg->wParam;
                NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)msg->lParam;
                if (bCalcValidRects)
                {
                    //这里去掉使用了自绘标题栏时顶部的白边
                    lpncsp->rgrc[0].top -= DPI(6);
                }
            }
        }
        break;
        case WM_NCACTIVATE:
        {
            if (CWinVersionHelper::IsWindows10OrLater() && d->m_ribbonOptionData.customTitleBar)
            {
                // 阻止默认窗口过程绘制非客户区边框
                HWND hWnd = (HWND)winId();
                return ::DefWindowProc(hWnd, WM_NCACTIVATE, msg->wParam, (LPARAM)-1);
            }
        }
        break;
        }
    }
#endif
    return QMainWindow::nativeEvent(eventType, message, result);
}

QAction *RibbonFrameWindow::_GetAction(const QString& strCmd) const
{
    auto iter = d->m_actionMap.find(strCmd);
    if (iter != d->m_actionMap.end())
        return iter.value();
    return nullptr;
}

QWidget *RibbonFrameWindow::_GetWidget(const QString& strCmd) const
{
    auto iter = d->m_widgetMap.find(strCmd);
    if (iter != d->m_widgetMap.end())
        return iter.value();
    return nullptr;
}

QMenu *RibbonFrameWindow::_GetMenu(const QString &strCmd) const
{
    auto iter = d->m_menuMap.find(strCmd);
    if (iter != d->m_menuMap.end())
        return iter.value();
    return nullptr;
}

void RibbonFrameWindow::SetItemIcon(const QString& strId, const QIcon& icon)
{
    QAction* pAction = _GetAction(strId);
    if (pAction != nullptr)
    {
        pAction->setIcon(icon);
    }
    else
    {
        QWidget* pWidget = _GetWidget(strId);
        QAbstractButton* pBtn = qobject_cast<QAbstractButton*>(pWidget);
        ImageLabel* pLabel = dynamic_cast<ImageLabel*>(pWidget);
        if (pBtn != nullptr)
            pBtn->setIcon(icon);
        if (pLabel != nullptr)
            pLabel->setIcon(icon);
    }
}

void RibbonFrameWindow::SetTabIndex(int index)
{
    if (d->showLeftNaviBar)
        d->m_pNavigateWidget->SetCurItem(index);
    else
        d->m_pTabWidget->setCurrentIndex(index);
}

int RibbonFrameWindow::GetTabIndex() const
{
    if (d->showLeftNaviBar)
        return d->m_pNavigateWidget->GetCurItem();
    else
        return d->m_pTabWidget->currentIndex();
}

void RibbonFrameWindow::SetDefaultWidget(QWidget* pWidget)
{
    //先移除之前的defaultWidget
    if (d->m_pDefaultWidget != nullptr)
        d->m_pStackedWidget->removeWidget(d->m_pDefaultWidget);
    //将defaultWidget添加到StackedWidget中
    d->m_pStackedWidget->addWidget(pWidget);
    d->m_pDefaultWidget = pWidget;
    OnTabIndexChanged(GetTabIndex());
}

IModule *RibbonFrameWindow::GetModule(const char *strModuleName) const
{
    auto iter = d->m_moduleNameMap.find(strModuleName);
    if (iter != d->m_moduleNameMap.end())
        return iter.value();
    return nullptr;
}

void RibbonFrameWindow::SetItemEnable(const char *strCmd, bool bEnable)
{
    QAction* pAction = _GetAction(strCmd);
    if (pAction != nullptr)
    {
        pAction->setEnabled(bEnable);
    }
    else
    {
        QWidget* pWidget = _GetWidget(strCmd);
        if (pWidget != nullptr)
            pWidget->setEnabled(bEnable);
    }
}

bool RibbonFrameWindow::IsItemEnable(const char *strCmd)
{
    QAction* pAction = _GetAction(strCmd);
    if (pAction != nullptr)
    {
        return pAction->isEnabled();
    }
    else
    {
        QWidget* pWidget = _GetWidget(strCmd);
        if (pWidget != nullptr)
            return pWidget->isEnabled();
    }
    return false;
}

void RibbonFrameWindow::SetItemChecked(const char *strCmd, bool checked)
{
    QAction* pAction = _GetAction(strCmd);
    if (pAction != nullptr)
    {
        pAction->setChecked(checked);
    }
    else
    {
        QAbstractButton* pBtn = qobject_cast<QAbstractButton*>(_GetWidget(strCmd));
        if (pBtn != nullptr)
            pBtn->setChecked(checked);
    }
}

bool RibbonFrameWindow::IsItemChecked(const char *strCmd)
{
    QAction* pAction = _GetAction(strCmd);
    if (pAction != nullptr)
    {
        return pAction->isChecked();
    }
    else
    {
        QAbstractButton* pBtn = qobject_cast<QAbstractButton*>(_GetWidget(strCmd));
        if (pBtn != nullptr)
            return pBtn->isChecked();
    }
    return false;
}

void *RibbonFrameWindow::SendModuleMessage(const char *moduleName, const char *msgType, void *para1, void *para2)
{
    //响应主题变化
    if (QString::fromUtf8(msgType) == MODULE_MSG_StyleChanged)
    {
        //主题变化时，如果功能区未显示，则重新调用ShowHideRibbon函数以更新ribbon标签的高度
        if (!d->m_ribbonOptionData.ribbonPin)
            SetRibbonPin(false);
    }

    //如果模块名为空，则向所有模块发送
    if (moduleName == nullptr || moduleName[0] == '\0')
    {
        Q_FOREACH(IModule* pModule, d->m_moduleNameMap)
        {
            if (pModule != nullptr)
                return pModule->OnMessage(msgType, para1, para2);
        }
    }
    //否则向指定模块发送
    else
    {
        IModule* pModule = GetModule(moduleName);
        if (pModule != nullptr)
            return pModule->OnMessage(msgType, para1, para2);
    }
    return nullptr;
}

const char* RibbonFrameWindow::GetItemText(const char* strId)
{
    static QByteArray itemText;
    itemText = "";
    QAction* pAction = _GetAction(strId);
    if (pAction != nullptr)
    {
        itemText = pAction->text().toUtf8();
    }
    QWidget* pWidget = _GetWidget(strId);
    if (pWidget != nullptr)
    {
        QAbstractButton* pBtn = qobject_cast<QAbstractButton*>(pWidget);
        if (pBtn != nullptr)
            itemText = pBtn->text().toUtf8();
        QLabel* pLable = qobject_cast<QLabel*>(pWidget);
        if (pLable != nullptr)
            itemText = pLable->text().toUtf8();
        QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(pWidget);
        if (pLineEdit != nullptr)
            itemText =  pLineEdit->text().toUtf8();
        QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(pWidget);
        if (pTextEdit != nullptr)
            itemText = pTextEdit->toPlainText().toUtf8();
        QComboBox* pComboBox = qobject_cast<QComboBox*>(pWidget);
        if (pComboBox != nullptr)
            itemText = pComboBox->currentText().toUtf8();
        ImageLabel* pImageLabel = dynamic_cast<ImageLabel*>(pWidget);
        if (pImageLabel != nullptr)
            itemText = pImageLabel->TextLabel()->text().toUtf8();
    }
    return itemText.constData();
}

void RibbonFrameWindow::SetItemText(const char* strId, const char* text)
{
    QAction* pAction = _GetAction(strId);
    if (pAction != nullptr)
    {
        pAction->setText(QString::fromUtf8(text));
    }
    else
    {
        QWidget* pWidget = _GetWidget(strId);
        if (pWidget != nullptr)
        {
            QAbstractButton* pBtn = qobject_cast<QAbstractButton*>(pWidget);
            if (pBtn != nullptr)
                pBtn->setText(QString::fromUtf8(text));
            QLabel* pLable = qobject_cast<QLabel*>(pWidget);
            if (pLable != nullptr)
                pLable->setText(QString::fromUtf8(text));
            QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(pWidget);
            if (pLineEdit != nullptr)
                pLineEdit->setText(QString::fromUtf8(text));
            QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(pWidget);
            if (pTextEdit != nullptr)
                pTextEdit->setPlainText(QString::fromUtf8(text));
            QComboBox* pComboBox = qobject_cast<QComboBox*>(pWidget);
            if (pComboBox != nullptr)
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
                pComboBox->setCurrentText(QString::fromUtf8(text));
#else
                pComboBox->setEditText(QString::fromUtf8(text));
#endif
            ImageLabel* pImageLabel = dynamic_cast<ImageLabel*>(pWidget);
            if (pImageLabel != nullptr)
                pImageLabel->TextLabel()->setText(QString::fromUtf8(text));
        }
    }
}

void RibbonFrameWindow::SetItemIcon(const char* strId, const char* iconPath, int iconSize)
{
    QIcon icon = RibbonFrameHelper::CreateIcon(QString::fromUtf8(iconPath), iconSize);
    SetItemIcon(QString::fromUtf8(strId), icon);
}

void* RibbonFrameWindow::GetAcion(const char* strId)
{
    return _GetAction(QString::fromUtf8(strId));
}

void* RibbonFrameWindow::GetWidget(const char* strId)
{
    return _GetWidget(QString::fromUtf8(strId));
}

void *RibbonFrameWindow::GetMenu(const char *strId)
{
    return _GetMenu(QString::fromUtf8(strId));
}

void RibbonFrameWindow::SetStatusBarText(const char* text, int timeOut)
{
    QStatusBar* pStatusBar = statusBar();
    if (pStatusBar != nullptr)
    {
        QString strText = QString::fromUtf8(text);
        if (!strText.isEmpty())
            pStatusBar->showMessage(strText, timeOut);
        else
            pStatusBar->clearMessage();
    }
}

int RibbonFrameWindow::GetItemCurIndex(const char* strId)
{
    QWidget* pWidget = _GetWidget(strId);
    QComboBox* pComboBox = qobject_cast<QComboBox*>(pWidget);
    if (pComboBox != nullptr)
    {
        return pComboBox->currentIndex();
    }
    QListWidget* pListWidget = qobject_cast<QListWidget*>(pWidget);
    if (pListWidget != nullptr)
    {
        return pListWidget->currentRow();
    }
    return -1;
}

void RibbonFrameWindow::SetItemCurIIndex(const char* strId, int index)
{
    QWidget* pWidget = _GetWidget(strId);
    QComboBox* pComboBox = qobject_cast<QComboBox*>(pWidget);
    if (pComboBox != nullptr)
    {
        pComboBox->setCurrentIndex(index);
        return;
    }
    QListWidget* pListWidget = qobject_cast<QListWidget*>(pWidget);
    if (pListWidget != nullptr)
    {
        pListWidget->setCurrentRow(index);
        return;
    }
}
