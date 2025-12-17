#ifndef RIBBONSTYLE_INTERFACE
#define RIBBONSTYLE_INTERFACE
#include <vector>
#include <string>

class IRibbonStyle
{
public:
    struct RGBColor
    {
        int r{};
        int g{};
        int b{};

        RGBColor(int _r, int _g, int _b)
            : r(_r), g(_g), b(_b)
        {}
    };

    virtual void GetAllStyleNames(std::vector<std::string>& styleNames) = 0;

    virtual const char* GetCurrentStyle() = 0;
    virtual void SetCurrentStyle(const char* styleName) = 0;

    //获取主题颜色
    virtual RGBColor GetThemeColor() = 0;
    virtual RGBColor GetThemeColor(int lightness) = 0;
    //设置主题颜色
    virtual void SetThemeColor(const RGBColor color) = 0;

    virtual void SetFollowingSystemThemeColor(bool followingSystemThemeColor) = 0;
    virtual bool IsFollowingSystemThemeColor() = 0;

    virtual void SetFollowingSystemColorMode(bool followingSystemColorMode) = 0;
    virtual bool IsFollowingSystemColorMode() = 0;
    
    //判断一个主题是否匹配当前系统深色/浅色主题
    virtual bool IsStyleMatchSystemColorMode(const char* styleName) = 0;
};

#endif