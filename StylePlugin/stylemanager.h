#ifndef CSTYLEMANAGER_H
#define CSTYLEMANAGER_H

#include <QString>
#include <QList>
#include <QWidget>
#include <QComboBox>
#include "themecolor.h"
#include <memory>
#include <functional>

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
    QList<std::shared_ptr<CStyle>> GetAllStyles();
    CStyle* GetStyle(const QString& styleName);

    //获取一个主题对应的深色主题
    CStyle* GetDarkStyle(const QString& styleName);

    //获取一个主题对应的浅色主题
    CStyle* GetLightStyle(const QString& styleName);

    //遍历所有主题
    //func返回true则终止遍历
    void IterateStyles(std::function<bool(std::shared_ptr<CStyle>)> func);

    static void ApplyQComboboxItemStyle(QComboBox* pCombobox);

    struct StylePair
    {
        std::shared_ptr<CStyle> lightStyle;
        std::shared_ptr<CStyle> darkStyle;
    };

public:
    QList<StylePair> m_styleList;
    CStyle m_defaultStyle;

private:
    CStyleManager();
    //添加一对主题
    void AddStylePair(const QString& lightStylePath, const QString& lightStyleName,
        const QString& darkStylePath = QString(), const QString& darkStyleName = QString());

    StylePair FindStylePair(const QString& styleName);

    static CStyleManager* m_instance;
};

#endif // CSTYLEMANAGER_H
