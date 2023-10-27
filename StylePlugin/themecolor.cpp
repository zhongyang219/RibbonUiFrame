#include "themecolor.h"

ThemeColor::ThemeColor()
{

}

void ThemeColor::SetColor(const QColor &color)
{
    m_color = color;
}

void ThemeColor::ApplyThemeColor(QString &strStyleSheet) const
{
    strStyleSheet.replace("@themeColorOri", OriginalColor().name());
    strStyleSheet.replace("@themeColor0", Color0().name());
    strStyleSheet.replace("@themeColorLighter1", Lighter1().name());
    strStyleSheet.replace("@themeColorLighter2", Lighter2().name());
    strStyleSheet.replace("@themeColorLighter3", Lighter3().name());
    strStyleSheet.replace("@themeColorLighter4", Lighter4().name());
    strStyleSheet.replace("@themeColorDarker1", Darker1().name());
    strStyleSheet.replace("@themeColorDarker2", Darker2().name());
    strStyleSheet.replace("@themeColorDarker3", Darker3().name());
    strStyleSheet.replace("@themeColorDarker4", Darker4().name());
}

QColor ThemeColor::OriginalColor() const
{
    return m_color;
}

QColor ThemeColor::Color0() const
{
    return ChangeLightness(128);
}

QColor ThemeColor::Lighter1() const
{
    return ChangeLightness(154);
}

QColor ThemeColor::Lighter2() const
{
    return ChangeLightness(180);
}

QColor ThemeColor::Lighter3() const
{
    return ChangeLightness(206);
}

QColor ThemeColor::Lighter4() const
{
    return ChangeLightness(232);
}

QColor ThemeColor::Darker1() const
{
    return ChangeLightness(102);
}

QColor ThemeColor::Darker2() const
{
    return ChangeLightness(76);
}

QColor ThemeColor::Darker3() const
{
    return ChangeLightness(50);
}

QColor ThemeColor::Darker4() const
{
    return ChangeLightness(24);
}

QColor ThemeColor::ChangeLightness(int lightness) const
{
    int h, s, l;
    m_color.getHsl(&h, &s, &l);
    QColor newColor;
    newColor.setHsl(h, s, lightness);
    return newColor;
}
