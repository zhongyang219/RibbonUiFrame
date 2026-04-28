#pragma once

#include <QAbstractNativeEventFilter>

// 本地事件过滤器类
class StyleEventFilter : public QAbstractNativeEventFilter
{
public:
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
#endif

    static void SetWindowDarkTheme(void* hWnd, bool darkTheme);


    // QAbstractNativeEventFilter interface
public:
};
