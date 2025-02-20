#include "RibbonFrameHelper.h"
#include "ribbonuipredefine.h"
#include <QToolButton>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>

bool RibbonFrameHelper::GetAttributeBool(const QDomElement& element, const QString& attr, bool defaultVal)
{
    QString strAttrValue = element.attribute(attr);
    if (!strAttrValue.isEmpty())
        return strAttrValue == "true" || strAttrValue.toInt() != 0;
    return defaultVal;
}

bool RibbonFrameHelper::IsSmallIcon(const QDomElement& element)
{
    if (element.hasAttribute("smallIcon"))
    {
        return GetAttributeBool(element, "smallIcon");
    }
    else
    {
        QString strBtnStyle = element.attribute("btnStyle");
        return strBtnStyle == "compact" || strBtnStyle == "textOnly" || strBtnStyle == "iconOnly";
    }
}

void RibbonFrameHelper::SetToolButtonStyle(QToolButton* pToolBtn, const QDomElement& element, Qt::ToolButtonStyle defaultStyle)
{
    if (pToolBtn != nullptr)
    {
        QString strBtnStyle = element.attribute("btnStyle");
        if (strBtnStyle == "compact")
        {
            pToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        }
        else if (strBtnStyle == "textOnly")
        {
            pToolBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
        else if (strBtnStyle == "iconOnly")
        {
            pToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        }
        else if (element.hasAttribute("smallIcon"))
        {
            bool smallIcon = GetAttributeBool(element, "smallIcon");
            if (!smallIcon)
            {
                pToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            }
            else
            {
                if (defaultStyle == Qt::ToolButtonIconOnly)
                    pToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
                else
                    pToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            }
        }
        else
        {
            pToolBtn->setToolButtonStyle(defaultStyle);
        }
    }
}

QString RibbonFrameHelper::GetElementId(const QDomElement& element)
{
    QString strId = element.attribute("id");
    if (strId.isEmpty())
        strId = element.attribute("commandID");
    return strId;
}

bool RibbonFrameHelper::GetElementEnabled(const QDomElement& element, bool defaultVal)
{
    if (element.hasAttribute("enabled"))
        return GetAttributeBool(element, "enabled", defaultVal);
    else if (element.hasAttribute("enable"))
        return GetAttributeBool(element, "enable", defaultVal);
    return defaultVal;
}

QIcon RibbonFrameHelper::CreateIcon(QString strPath, int size)
{
    if (!strPath.isEmpty())
    {
        //如果图标路径不是资源路径且文件不存在，则在路径前面加上应用程序目录
        if (!strPath.startsWith(":/") && !QFileInfo(strPath).isFile())
            strPath = qApp->applicationDirPath() + '/' + strPath;
        QPixmap pixmap(strPath);
        if (!pixmap.isNull())
            return QIcon(pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        else
            qDebug() << QString(u8"加载图标“%1”失败！").arg(strPath);
    }
    return QIcon();
}

bool RibbonFrameHelper::IsToolBarTag(const QString& tagName)
{
    return tagName == "ToolBar" || tagName == "Toolbar";
}

bool RibbonFrameHelper::IsActionTag(const QString& tagName)
{
    return tagName == "Action" || tagName == "SysBarAction";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RibbonFramePrivate::RibbonFramePrivate(const QStringList& cmdLine)
{
    m_pNoMainWindowLabel = new QLabel(QSTR("模块未提供主窗口。"));
    m_pModuleLoadFailedLabel = new QLabel(QSTR("模块加载失败。"));
    m_pNoMainWindowLabel->setAlignment(Qt::AlignCenter);
    m_pModuleLoadFailedLabel->setAlignment(Qt::AlignCenter);

    //解析命令行参数
    m_isModuleManager = cmdLine.contains("-module_manager");
}

void RibbonFramePrivate::LoadConfig()
{
    //载入已禁用模块设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    m_disabledModulePath = settings.value("disabledModulePath").toStringList().toSet();
    //恢复导航栏的宽度
    if (m_pNaviSplitter != nullptr)
    {
        QStringList strSizes = settings.value("naviBarWidth", 0).toString().split(',');
        QList<int> sizes;
        for (const QString& str : strSizes)
        {
            int s = str.toInt();
            if (s > 0)
                sizes.push_back(s);
        }
        if (!sizes.isEmpty())
            m_pNaviSplitter->setSizes(sizes);
    }
}

void RibbonFramePrivate::SaveConfig() const
{
    //保存禁用插件设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("disabledModulePath", QStringList(m_disabledModulePath.toList()));
    //保存导航栏的宽度
    if (m_pNaviSplitter != nullptr && !m_pNaviSplitter->sizes().isEmpty())
    {
        QStringList strSizes;
        auto sizes = m_pNaviSplitter->sizes();
        for (const int s : sizes)
            strSizes.push_back(QString::number(s));
        settings.setValue("naviBarWidth", strSizes.join(','));
    }
}

bool RibbonFramePrivate::IsModuleManager() const
{
    return m_isModuleManager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ImageLabel::ImageLabel(QWidget* pParent, const QString& iconPath, const QString& text)
    : QWidget(pParent)
{
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    pImageLabel = new QLabel(this);
    QPixmap iconPixmap(iconPath);
    pImageLabel->setPixmap(iconPixmap.scaled(ICON_SIZE_S, ICON_SIZE_S, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    pLayout->addWidget(pImageLabel);
    pTextLabel = new QLabel(this);
    pTextLabel->setText(text);
    pLayout->addWidget(pTextLabel, 1);
}

void ImageLabel::setIcon(const QIcon& icon)
{
    pImageLabel->setPixmap(icon.pixmap(ICON_SIZE_S, ICON_SIZE_S));
}

QLabel* ImageLabel::TextLabel() const
{
    return pTextLabel;
}
