#Cleps Video Player CmakeList
cmake_minimum_required(VERSION 2.8.11)
project(cleps)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(Qt5DBus)

set(cleps_SOURCES main.cpp
        cleps_vidplayer.cpp 
    Cleps_MainWindow.cpp 
    volumeslider.cpp 
    playlistview.cpp 
    settings_dialog.cpp
    shortcuteditor.cpp 
    shortcuts.cpp 
    behaviour_settings.cpp 
    bookmarkmanager.cpp)
    
set(cleps_Headers cleps_vidplayer.h 
    Cleps_MainWindow.h 
    volumeslider.h 
    playlistview.h 
    settings_dialog.h
    shortcuteditor.h 
    shortcuts.h 
    behaviour_settings.h 
    bookmarkmanager.h)

set(cleps_RESOURCES icons.qrc)

add_executable(cleps ${cleps_SOURCES} ${cleps_Headers} ${cleps_RESOURCES})
target_link_libraries(cleps Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::DBus)
