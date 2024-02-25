cmake_minimum_required(VERSION 3.16)
project(uglobalhotkey)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")


find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)

include_directories(${Qt${QT_VERSION_MAJOR}Quick_PRIVATE_INCLUDE_DIRS})
include_directories("qt6/QtGui/6.4.2/QtGui/")

set(SRCS hotkeymap.h uexception.cpp uexception.h uglobal.h uglobalhotkeys.cpp uglobalhotkeys.h ukeysequence.cpp ukeysequence.h)

add_library(uglobalhotkey ${SRCS})
target_link_libraries(uglobalhotkey PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia xcb xcb-keysyms)