#ifndef RIBBONSTYLE_INTERFACE
#define RIBBONSTYLE_INTERFACE

#include <QStringList>
#include <QColor>

class IRibbonStyle
{
public:
    virtual void GetAllStyleNames(QStringList& styleNames) = 0;

    virtual QString GetCurrentStyle() = 0;
    virtual void SetCurrentStyle(const QString& styleName) = 0;

    virtual QColor GetThemeColor() = 0;
    virtual void SetThemeColor(QColor color) = 0;

    virtual void SetFollowingSystemThemeColor(bool followingSystemThemeColor) = 0;
    virtual bool IsFollowingSystemThemeColor() = 0;

    virtual void SetFollowingSystemColorMode(bool followingSystemColorMode) = 0;
    virtual bool IsFollowingSystemColorMode() = 0;
    
    //判断一个主题是否匹配当前系统深色/浅色主题
    virtual bool IsStyleMatchSystemColorMode(const QString& styleName) = 0;
};

#endif