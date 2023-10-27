#ifndef RIBBONUIPREDEFINE_H
#define RIBBONUIPREDEFINE_H

#include <QtGlobal>
#include <QString>

//使用Unicode方式从字面值创建QString对象，以避免中文出现乱码
#define QSTR(str) QString::fromWCharArray(L ## str)

#define SCOPE_NAME "Apps By ZhongYang"

//命令id定义
#define CMD_AppExit "AppExit"
#define CMD_AppAboutQt "AppAboutQt"
#define CMD_RibbonPin "RibbonPin"
#define CMD_RibbonOptions "RibbonOptions"
#define CMD_DefaultStyle "DefaultStyle"

//模块消息定义
#define MODULE_MSG_GetStyleType "GetStyleType"      //获取当前主题样式类别，向StylePlugin模块发送，返回CStyleManager::StyleType类型
#define MODULE_MSG_GetStyleName "GetStyleName"      //获取当前主题样式名称，向StylePlugin模块发送，返回const char*类型
#define MODULE_MSG_IsDarkTheme "IsDarkTheme"        //获取当前主题是否为深色主题，向StylePlugin模块发送，返回bool类型
#define MODULE_MSG_StyleChanged "StyleChanged"      //由StylePlugin模块发出，通知主题样式已改变。para1：样式名称，const char*类型
#define MODULE_MSG_SetThemeColor "SetThemeColor"    //设置当前主题颜色，向StylePlugin模块发送，参数为主题颜色，const char*类型，主题颜色的十六进制值，如#557eef，可以通过QColor::name函数得到

#include <QGuiApplication>
#include <QScreen>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//将一个像素值根据当前屏幕DPI设置进行等比放大
static int DPI(int x)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    return x;
#else
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() * x / 96;
#endif
}


#endif // RIBBONUIPREDEFINE_H
