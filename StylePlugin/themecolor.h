#ifndef THEMECOLOR_H
#define THEMECOLOR_H
#include <QColor>

class ThemeColor
{
public:
    ThemeColor();
    void SetColor(const QColor& color);

    void ApplyThemeColor(QString& strStyleSheet) const;

    QColor OriginalColor() const;   //原始主题色
    QColor Color0() const;          //亮度为128的颜色
    QColor Lighter1() const;
    QColor Lighter2() const;
    QColor Lighter3() const;
    QColor Lighter4() const;
    QColor Darker1() const;
    QColor Darker2() const;
    QColor Darker3() const;
    QColor Darker4() const;

    /**
     * @brief       更改当前主题色的亮度值
     * @param[in]	lightness 新的亮度值
     * @return      更改亮度值后的颜色
     */
    QColor ChangeLightness(int lightness) const;

private:
    QColor m_color; //当前主题色
};

#endif // THEMECOLOR_H
