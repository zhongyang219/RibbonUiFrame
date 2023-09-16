#ifndef RIBBONUIPREDEFINE_H
#define RIBBONUIPREDEFINE_H

//使用Unicode方式从字面值创建QString对象，以避免中文出现乱码
#define QSTR(str) QString::fromWCharArray(L ## str)

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
