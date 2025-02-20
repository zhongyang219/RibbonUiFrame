#ifndef CSTYLEMANAGER_H
#define CSTYLEMANAGER_H

#include <QString>
#include <QList>
#include <QWidget>
#include <QComboBox>
#include "themecolor.h"

class CStyleManager
{
public:
    static CStyleManager* Instance();

    class CStyle
    {
    public:
        enum StyleType
        {
            Light,
            Dark
        };

        CStyle(const QString& strPath, const QString name, StyleType type, bool bParsePaletteColor = true);
        /*
         * 函数说明: 为一个窗口应用样式表
         * 输入参数:
         *   pWidget: 窗口的指针
         */
        void ApplyStyleSheet(QWidget* pWidget = nullptr, ThemeColor* pThemeColor = nullptr) const;

        /**
            @brief 对一个样式表中所有像素值根据系统DPI设定进行放大
            @param strStyle - 样式表
        **/
        static void StyleDpiChange(QString& strStyle);

    public:
        QString m_strName;
        StyleType m_type;

    private:
        QString m_strQss;             //样式表
        QPalette m_palette;           //调色板
        bool m_bParsePaletteColor;
    };

    void ApplyStyleSheet(const QString& styleName, QWidget* pWidget = nullptr, ThemeColor* pThemeColor = nullptr);
    const QList<CStyle>& GetAllStyles() const;
    CStyle* GetStyle(const QString& styleName);

    static void ApplyQComboboxItemStyle(QComboBox* pCombobox);

public:
    QList<CStyle> m_styleList;
    CStyle m_defaultStyle;

private:
    CStyleManager();

    static CStyleManager* m_instance;
};

#endif // CSTYLEMANAGER_H
