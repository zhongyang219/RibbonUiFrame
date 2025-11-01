#include "Config.h"
#include <QApplication>

CConfig::CConfig(const QString& strModuleName)
{
#ifdef Q_OS_WIN
    QString strConfigPath = QString("HKEY_CURRENT_USER\\Software\\Apps By ZhongYang\\%1").arg(qApp->applicationName());
#ifdef QT_DEBUG
    strConfigPath += " (Debug)";
#endif
    if (!strModuleName.isEmpty())
    {
        strConfigPath += '\\';
        strConfigPath += strModuleName;
    }
    m_settings = new QSettings(strConfigPath, QSettings::NativeFormat);
#else
    m_settings = new QSettings("Apps By ZhongYang", qApp->applicationName());
#endif
}

CConfig::~CConfig()
{
    delete m_settings;
}

void CConfig::WriteValue(const QString& strKeyName, const QVariant& value)
{
    if (value.type() == QVariant::Bool)     //由于bool类型保存到注册表会转换成字符串，这里将它转换成int
        m_settings->setValue(strKeyName, value.toInt());
    else
        m_settings->setValue(strKeyName, value);
}

QVariant CConfig::GetValue(const QString& strKeyName, const QVariant& defaultValue) const
{
    return m_settings->value(strKeyName, defaultValue);
}
