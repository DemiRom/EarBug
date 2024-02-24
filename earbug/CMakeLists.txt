cmake_minimum_required(VERSION 3.16)
project(EarBug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")


find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)
# Find the PulseAudio library
find_library(PULSEAUDIO_LIBRARY NAMES pulse)

# Check if the library was found
if(NOT PULSEAUDIO_LIBRARY)
    message(FATAL_ERROR "Could not find PulseAudio library.")
endif()

include_directories(../uglobalhotkey)

set(SRCS
        BaseTheme.h
        main.cpp
        EarBug.qrc
        FormMainWindow.cpp
        FormMainWindow.h
        FormMainWindow.ui
        WidgetAppVolumeControl.cpp
        WidgetAppVolumeControl.h
        WidgetAppVolumeControl.ui
        FormSettingsWindow.cpp
        FormSettingsWindow.h
        FormSettingsWindow.ui
        PulseAudioController.cpp
        PulseAudioController.h
        EarBugWindowPosition.h
        GlobalSettingsManager.cpp
        GlobalSettingsManager.h
        Hotkey.h
        HotkeySettingsWidget.cpp
        HotkeySettingsWidget.h
        HotkeySettingsWidget.ui
        EarBug.cpp
        EarBug.h
        DebugLogger.h
)

# Add exe here
if (WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "resources/appicon.rc")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_executable(EarBug ${SRCS} ${APP_ICON_RESOURCE_WINDOWS})
    else ()
        add_executable(EarBug WIN32 ${SRCS} ${APP_ICON_RESOURCE_WINDOWS})
    endif ()
elseif (APPLE)
    # NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
    # the property added to Info.plist
    set(MACOSX_BUNDLE_ICON_FILE earbug.icns)

    # And this part tells CMake where to find and install the file itself
    set(app_ICONS ${CMAKE_CURRENT_SOURCE_DIR}/resources/earbug.icns)
    set_source_files_properties(${app_ICONS} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(EarBug MACOSX_BUNDLE ${app_ICONS} ${SRCS})
else (UNIX AND NOT APPLE)
    qt_add_executable(EarBug ${SRCS})
endif ()

target_link_libraries(EarBug PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia pulse uglobalhotkey)

if (WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release" AND NOT MXE_BUILD)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT_INSTALL_PATH}/bin/windeployqt.exe --dir $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.exe)
elseif (WIN32 AND CMAKE_BUILD_TYPE MATCHES "Debug" AND NOT MXE_BUILD)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT_INSTALL_PATH}/bin/windeployqt.exe --debug --dir $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.exe)
elseif (APPLE)
    install(TARGETS EarBug RUNTIME DESTINATION bin BUNDLE DESTINATION bin)
else ()
    install(TARGETS EarBug RUNTIME DESTINATION bin)
endif ()