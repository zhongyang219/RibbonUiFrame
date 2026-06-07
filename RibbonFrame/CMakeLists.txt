cmake_minimum_required(VERSION 3.16)
project(RibbonFrame LANGUAGES CXX)

# 设置C++标准和Qt自动处理
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${PROJECT_NAME} SHARED
    ../include/mainframe_global.h
    ../include/mainframeinterface.h
    ../include/moduleinterface.h
    ../include/ribbonframewindow.h
    ../include/ribbonuipredefine.h
    RibbonFrameHelper.h
    modulemanagerdlg.h
    settingsdialog.h
    toolkits/WinVersionHelper.h
    toolkits/moduletools.h
    widgets/navigatewidget.h
    widgets/titlebarwidget.h
    RibbonFrameHelper.cpp
    modulemanagerdlg.cpp
    ribbonframewindow.cpp
    settingsdialog.cpp
    toolkits/WinVersionHelper.cpp
    toolkits/moduletools.cpp
    widgets/navigatewidget.cpp
    widgets/titlebarwidget.cpp
    ribbonframe.qrc
    RibbonFrame.rc
    modulemanagerdlg.ui
    settingsdialog.ui
    qxframeless/framelesshelper.h
    qxframeless/framelesshelper_p.h
    qxframeless/qxframeless_global.h
    qxframeless/framelesshelper.cpp
)

# 依赖的Qt组件
set(QT_COMPONENTS Core Gui Widgets Xml)
# 尝试先找到Qt6，如果找不到再尝试Qt5
find_package(Qt6 COMPONENTS ${QT_COMPONENTS} QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 5.15 COMPONENTS ${QT_COMPONENTS} REQUIRED)
    set(QT_VERSION_MAJOR 5)
endif()

# 将Qt组件转换为链接目标，根据Qt版本自动添加前缀
list(TRANSFORM QT_COMPONENTS PREPEND "Qt${QT_VERSION_MAJOR}::" OUTPUT_VARIABLE QT_LINK_TARGETS)

# 包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 编译宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE RIBBONFRAME_LIBRARY QX_FRAMELESS_LIBRARY)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE QX_FRAMELESS_NATIVE)
endif()

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LINK_TARGETS}
)
