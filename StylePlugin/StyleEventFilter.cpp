#include "StyleEventFilter.h"
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif
#include <QByteArray>
#include "styleplugin.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool StyleEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_CREATE || msg->message == WM_INITDIALOG)
        {
            // 窗口创建时设置深色标题栏
            SetWindowDarkTheme(msg->hwnd, StylePlugin::Instance()->IsDarkTheme());
        }
    }
#endif
    return false; // 返回 false，表示事件继续传递
}

void StyleEventFilter::SetWindowDarkTheme(void* hWnd, bool darkTheme)
{
#ifdef Q_OS_WIN
    BOOL darkMode = darkTheme;
    DwmSetWindowAttribute((HWND)hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
#endif
}
