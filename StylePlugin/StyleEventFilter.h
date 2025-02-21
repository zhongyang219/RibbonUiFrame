#pragma once

#include <QAbstractNativeEventFilter>

// 本地事件过滤器类
class StyleEventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

    static void SetWindowDarkTheme(void* hWnd, bool darkTheme);

};
