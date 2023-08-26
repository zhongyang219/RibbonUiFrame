#ifndef RIBBONFRAMEWINDOW_H
#define RIBBONFRAMEWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QString>
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include "mainframe_global.h"

class QDomElement;
class QToolBar;
class QToolButton;
class QVBoxLayout;
class MainFramePrivate;

class RIBBONFRAME_EXPORT RibbonFrameWindow : public QMainWindow, public IMainFrame
{
    Q_OBJECT

public:
    RibbonFrameWindow(QWidget *parent = nullptr);
    virtual ~RibbonFrameWindow();

signals:

private slots:
    void OnTabIndexChanged(int index);      //响应标签切换
    void OnActionTriggerd(bool checked);    //响应任意一个工具栏中的按钮被点击

private:
    void LoadUIFromXml();           //从xml文件加载界面
    IModule* LoadPlugin(const QString &strModulePath);
    void LoadMainFrameUi(const QDomElement& element);   //从一个xml节点加载界面
    void LoadUiElement(const QDomElement& element, QToolBar* pToolbar);     //加载一组UI元素（用于Ribbin的Page）
    void LoadSimpleToolbar(const QDomElement& element, QToolBar* pToolbar);   //加载一组Action元素，图标全部为小图标（用于快速启动栏）

    QAction* LoadUiAction(const QDomElement& element);    //从一个xml节点加载Action
    QWidget* LoadUiWidget(const QDomElement& element, QWidget* pParent, bool& smallIcon); //从一个xml节点加载Widget
    QMenu* LoadUiMenu(const QDomElement& element);      //从一个xml节点加载菜单

    /**
     * @brief       向界面添加一个控件
     * @param[in]	pUiWidget 添加的控件
     * @param[in]	smallIcon 是否为小图标
     * @param[in]	pToolbar 要添加控件的工具栏
     * @param[in][out]	previousLayout 当控件为小图标时垂直排列的布局
     */
    void AddUiWidget(QWidget* pUiWidget, bool smallIcon, QToolBar* pToolbar, QVBoxLayout*& previousLayout);       //向工具栏界面添加一个控件

    void InitMenuButton(QToolButton* pMenuBtn, const QDomElement& element);     //根据一个xml节点设置菜单按钮的状态

    void ApplyRadioButtonGroup();

    QWidget* GetModuleMainWindow(IModule* pModule);  //获取模块的主窗口

protected:
    IModule* CurrentModule() const;

    /**
     * @brief       响应一个命令。
     * @note        当点击工具栏中的Action、CheckBox或RadioButton时会触发此函数。重写此函数用于响应需要在主窗口中响应的命令，
     *              需要在模块中响应的命令请在对应模块的OnCommand函数中响应。如果派生类重写了此函数，请在重写的函数中调用基类的函数。
     * @param[in]	strCmd：命令的ID
     * @param[in]	checked：命令是否选中（如果命令具有可选中属性时）
     * @return      返回true表示命令已经被响应。如果命令需要在模块中响应，请返回false。
     */
    virtual bool OnCommand(const QString& strCmd, bool checked);

    /**
     * @brief       显示一个提示消息
     * @param[in]	msgTitle：消息标题
     * @param[in]	msgBody：消息体
     * @param[in]	msgType：消息类型，同QMessageBox::Icon枚举定义
     */
    void ShowMessage(const QString& msgTitle, const QString& msgBody, int msgType);

    QAction *_GetAction(const QString& strCmd) const;
    QWidget *_GetWidget(const QString& strCmd) const;

private:
    MainFramePrivate* d;        //私有成员变量，定义在cpp文件中

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent*) override;

    // IMainFrame interface
public:
    virtual IModule *GetModule(const char* strModuleName) const override;
    virtual void SetItemEnable(const char* strCmd, bool bEnable) override;
    virtual bool IsItemEnable(const char* strCmd) override;
    virtual void SetItemChecked(const char* strCmd, bool checked) override;
    virtual bool IsItemChecked(const char* strCmd) override;
    virtual void* SendModuleMessage(const char* moduleName, const char* msgType, void* para1 = nullptr, void* para2 = nullptr) override;
    virtual const char* GetItemText(const char* strId) override;
    virtual void SetItemText(const char* strId, const char* text) override;
    virtual void SetItemIcon(const char* strId, const char* iconPath, int iconSize) override;
    virtual void* GetAcion(const char* strId) override;
    virtual void* GetWidget(const char* strId) override;
};

#endif // RIBBONFRAMEWINDOW_H
