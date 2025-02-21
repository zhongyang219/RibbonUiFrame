#include "stylemanager.h"
#include <QFile>
#include <QPalette>
#include <QStyledItemDelegate>
#include <QApplication>
#include "StringHelper.h"
#include <QScreen>
#include "ribbonuipredefine.h"

CStyleManager::CStyle::CStyle(const QString &strPath, const QString name, StyleType type, bool bParsePaletteColor)
    : m_strName(name),
      m_type(type),
      m_bParsePaletteColor(bParsePaletteColor)
{
    //载入样式表文件
    QFile file(strPath);

    if (file.open(QFile::ReadOnly))
    {
        m_strQss = QString::fromUtf8(file.readAll());
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
        StyleDpiChange(m_strQss);
#endif
        if (bParsePaletteColor)
        {
            QString paletteColor = m_strQss.mid(20, 7);
            QString textColor = m_strQss.mid(49, 7);
            m_palette = QPalette(paletteColor);
            m_palette.setColor(QPalette::Text, textColor);
        }
        file.close();
    }
}

void CStyleManager::CStyle::ApplyStyleSheet(QWidget *pWidget, ThemeColor *pThemeColor) const
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString strQss = m_strQss;
    if (pThemeColor != nullptr)
    {
        pThemeColor->ApplyThemeColor(strQss);
//        qDebug() << "theme color:" << pThemeColor->OriginalColor().name();
    }

    if (pWidget != nullptr)
    {
        //设置样式表
        if (m_bParsePaletteColor)
            pWidget->setPalette(m_palette);
        pWidget->setStyleSheet("");
        pWidget->setStyleSheet(strQss);
    }
    else
    {
        if (m_bParsePaletteColor)
            qApp->setPalette(m_palette);
        qApp->setStyleSheet("");
        qApp->setStyleSheet(strQss);
    }
    QApplication::restoreOverrideCursor();
}

/**
 * @brief       查找qss样式表中“px”的第一个数字的位置
 * @param[in]	str qss样式表
 * @param[in]	index “px”的位置
 * @return      px第一个数字的位置
 */
static int FindPxStart(const QString& str, int index)
{
    for (int i = index; i >= 0; i--)
    {
        if (!StringHelper::IsNumber(str[i].toLatin1()))
            return i + 1;
    }
    return -1;
}

void CStyleManager::CStyle::StyleDpiChange(QString &strStyle)
{
    int index = 0;
    while (true)
    {
        //查找“px”
        index = strStyle.indexOf("px", index + 1);
        if (index < 0)
            break;
        //查找像素的起始位置
        int indexStart = FindPxStart(strStyle, index - 1);
        //获取像素值
        QString strPixel = strStyle.mid(indexStart, index - indexStart);
        int pixel = strPixel.toInt();
        //对像素值进行DPI转换
        int newPixel = DPI(pixel);
        //判断是否为QRadioButton的border-radius
        //这里用于解决QRadioButton的border-radius在经过DPI放大后可能会超过其大小导致border-radius属性失效的问题
        if (StringHelper::IsStringContainsForword(strStyle, "border-radius", '\n', index) && StringHelper::IsStringContainsForword(strStyle, "QRadioButton", '}', index))
        {
            int radioBtnSize = DPI(12);         //QRadioButton的大小固定为12
            if (radioBtnSize % 2 == 0)          //如果12像素经过DPI放大后的值为偶数。则border-radius的值应该为它的一半
                newPixel = radioBtnSize / 2;
            else                                //否则，border-radius的值应该为它加1的一半
                newPixel = (radioBtnSize + 1) / 2;
        }
        //更新样式表
        QString strNewPixel = QString::number(newPixel);
        strStyle.replace(indexStart, strPixel.size(), strNewPixel);
        //更新查找位置
        index = indexStart + strNewPixel.size() + 2;
    }
//#ifdef QT_DEBUG
//    //Debug模式下将更改后的样式写入文件
//    QFile file("./styleChange.css");
//    if (file.open(QIODevice::WriteOnly))
//    {
//        file.write(strStyle.toUtf8());
//        file.close();
//    }
//#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CStyleManager* CStyleManager::m_instance;

CStyleManager *CStyleManager::Instance()
{
    if (m_instance == nullptr)
        m_instance = new CStyleManager();
    return m_instance;
}

void CStyleManager::ApplyStyleSheet(const QString &styleName, QWidget *pWidget, ThemeColor* pThemeColor)
{
    for (auto iter = m_styleList.begin(); iter != m_styleList.end(); ++iter)
    {
        if (iter->m_strName == styleName)
        {
            iter->ApplyStyleSheet(pWidget, pThemeColor);
            break;
        }
    }
}

const QList<CStyleManager::CStyle> &CStyleManager::GetAllStyles() const
{
    return m_styleList;
}

CStyleManager::CStyle *CStyleManager::GetStyle(const QString &styleName)
{
    for (auto iter = m_styleList.begin(); iter != m_styleList.end(); ++iter)
    {
        if (iter->m_strName == styleName)
        {
            return &(*iter);
        }
    }
    return nullptr;
}

void CStyleManager::ApplyQComboboxItemStyle(QComboBox *pCombobox)
{
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    pCombobox->setItemDelegate(itemDelegate);
}

CStyleManager::CStyleManager()
    : m_defaultStyle(":/qss/default_style.qss", CMD_DefaultStyle, CStyle::Light, false)
{
/*
    m_styleList.push_back(CStyle(":/qss/silvery.css", QSTR("银色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/blue.css", QSTR("蓝色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/lightblue.css", QSTR("浅蓝色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/darkblue.css", QSTR("深蓝色"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/gray.css", QSTR("灰色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/lightgray.css", QSTR("浅灰色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/darkgray.css", QSTR("深灰色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/black.css", QSTR("黑色"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/lightblack.css", QSTR("浅黑色"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/darkblack.css", QSTR("深黑色"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/psblack.css", QSTR("PS黑色"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/bf.css", QSTR("深黑色2"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/test.css", QSTR("紫色"), CStyle::Dark));
*/
    m_styleList.push_back(CStyle(":/qss/flatblack.css", QSTR("黑色扁平"), CStyle::Dark));
    m_styleList.push_back(CStyle(":/qss/flatwhite.css", QSTR("白色扁平"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/flatwhite2.css", QSTR("现代白色扁平"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2010blue.css", QSTR("Office2010蓝色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2010silvery.css", QSTR("Office2010银色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2010black.css", QSTR("Office2010黑色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2013white.css", QSTR("Office2013白色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2013lightgray.css", QSTR("Office2013亮灰"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2013darkgray.css", QSTR("Office2013黑灰"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2016.css", QSTR("Office2016彩色"), CStyle::Light));
    m_styleList.push_back(CStyle(":/qss/offece2016dark.css", QSTR("Office2016深色"), CStyle::Dark));
#ifdef QT_DEBUG
    m_styleList.push_back(CStyle(":/qss/windows10light.css", QSTR("Windows10"), CStyle::Light));
#endif
}
