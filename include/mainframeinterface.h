#ifndef MAINWINDOWINTERFACE
#define MAINWINDOWINTERFACE

class IModule;
class IRibbonStyle;
class IService;

//界面框架接口
//接口中所有const char*类型均为UTF8编码。
class IMainFrame
{
public:
    /**
     * @brief       设置主窗口Ribbin工具栏命令的启用/禁用状态
     * @param[in]	strId: 命令的ID
     * @param[in]	enable: 是否启用
     */
    virtual void SetItemEnable(const char* strId, bool enable) = 0;

    /**
     * @brief       判断主窗口Ribbin工具栏的一个命令是否被被启用
     * @param[in]	strId: 命令的ID
     * @return      是否被启用
     */
    virtual bool IsItemEnable(const char* strId) = 0;

    /*
     * 函数说明: 设置主窗口Ribbin工具栏命令的选中状态
     * 输入参数:
     *   strId: 命令的ID
     *   enable: 是否选中
     */
    virtual void SetItemChecked(const char* strId, bool checked) = 0;

    /**
     * @brief       判断主窗口Ribbin工具栏的一个命令是否被选中
     * @param[in]	strId: 命令的ID
     * @return      是否被选中
     */
    virtual bool IsItemChecked(const char* strId) = 0;

    /**
     * @brief       获取Ribbon标签中一个元素的文本
     * @param[in]	strId: 命令的ID
     * @return      获取的文本
     */
    virtual const char* GetItemText(const char* strId) = 0;

    /**
     * @brief       设置Ribbon标签中一个元素的文本
     * @param[in]	strId: 命令的ID
     * @param[in]	text: 设置的文本
     */
    virtual void SetItemText(const char* strId, const char* text) = 0;

    /**
    * @brief       设置Ribbon标签中一个元素的图标
    * @param[in]	strId: 命令的ID
    * @param[in]	iconPath: 图标的路径
    * @param[in]	iconSize: 图标的大小
    */
    virtual void SetItemIcon(const char* strId, const char* iconPath, int iconSize) = 0;

    /**
     * @brief       获取Ribbon标签中Combox或ListWidget当前选中项
     * @param[in]	strId: 命令的ID
     * @return      选中项的序号
     */
    virtual int GetItemCurIndex(const char* strId) = 0;

    /**
    * @brief       设置Ribbon标签中Combox或ListWidget当前选中项
    * @param[in]   strId: 命令的ID
    * @param[in]   index: 选中项的序号
    */
    virtual void SetItemCurIIndex(const char* strId, int index) = 0;

    /**
     * @brief       根据模块名称获取模块对象的指针
     * @param[in]	moduleName 模块名称，由模块接口GetModuleName()返回。
     * @return      模块对象的指针
     */
    virtual IModule *GetModule(const char* strModuleName) const = 0;

    /**
     * @brief       向一个模块发送消息
     * @param[in]	moduleName 模块名称（如果为nullptr或空字符，则向所有模块发送）
     * @param[in]	msgType 消息类型
     * @param[in]	para1 para2 可选参数
     * @return      返回的消息
     */
    virtual void* SendModuleMessage(const char* moduleName, const char* msgType, void* para1 = nullptr, void* para2 = nullptr) = 0;

    /**
     * @brief       从主窗口Ribbin工具栏获取一个命令
     * @param[in]	strId: 命令的ID
     * @return      命令的QAction指针
     */
    virtual void* GetAcion(const char* strId) = 0;
	
    /**
     * @brief       从主窗口Ribbin工具栏获取一个控件
     * @param[in]	strId: 命令的ID
     * @return      控件的QWidget指针（如果是Action则返回QToolButton的指针）
     */
    virtual void* GetWidget(const char* strId) = 0;

    /**
     * @brief       从主窗口Ribbin工具栏获取一个菜单
     * @param[in]	strId: 菜单的ID
     * @return      菜单的QMenu指针
     */
    virtual void* GetMenu(const char* strId) = 0;

    /**
     * @brief       设置状态栏文本
     * @param[in]	text 要设置的文本
     * @param[in]	timeOut 文本显示的时长，单位为毫秒，0表示一直显示
     */
    virtual void SetStatusBarText(const char* text, int timeOut) = 0;

    /**
     * @brief 获取当前Ribbon主题接口
     * @return 
     */
    virtual IRibbonStyle* GetRibbonStyle() = 0;

    /**
     * @brief 注册一个服务
     * @param className 服务类的类名
     * @param pService 要注册服务类的对象（注册时需要new一个服务类的对象，该由智能指针管理，不能手动释放，不能传递栈上的对象）
     * @return 如果服务已注册则返回false，否则返回true
     */
    virtual bool RegisterService(const char* className, IService* pService) = 0;

    /**
     * @brief 获取一个已注册的服务类对象
     * @param className 服务类的类名
     * @return 如果存在返回对象的指针，否则返回空指针
     */
    virtual IService* GetService(const char* className) const = 0;

};

#endif // MAINWINDOWINTERFACE

