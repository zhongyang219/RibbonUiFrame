cmake_minimum_required(VERSION 3.16)
project(MainApp LANGUAGES CXX)

# 设置C++标准和Qt自动处理
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 仅在 macOS 桌面平台下注入 MACOSX_BUNDLE 标志
if(APPLE)
    set(QT_BUNDLE_FLAG MACOSX_BUNDLE)
else()
    set(QT_BUNDLE_FLAG "")
endif()

# macOS下的图标
set(MACOS_APP_ICON_PATH "res/logo.icns")
get_filename_component(MACOS_APP_ICON_NAME ${MACOS_APP_ICON_PATH} NAME)

set(APP_SOURCES
    mainwindow.h
    toolkits/Config.h
    main.cpp
    mainwindow.cpp
    toolkits/Config.cpp
    res/res.rc
    resource.qrc
)

# macOS 平台追加 .icns 图标文件到源列表
if(APPLE)
    list(APPEND APP_SOURCES ${MACOS_APP_ICON_PATH})
endif()

add_executable(${PROJECT_NAME} ${QT_BUNDLE_FLAG} ${APP_SOURCES})

# 依赖的Qt组件
set(QT_COMPONENTS Core Gui Widgets)
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

# 输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    # 可执行程序输出路径
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin/debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin/release
)

# macOS 平台配置 Bundle 图标与资源打包路径
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_ICON_FILE ${MACOS_APP_ICON_NAME}
        MACOSX_BUNDLE_INFO_PLIST ""
    )
    set_source_files_properties(${MACOS_APP_ICON_PATH} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

# 链接目录与库
target_link_directories(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../bin/debug>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../bin/release>
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    RibbonFrame
    ${QT_LINK_TARGETS}
)
