
cmake_minimum_required(VERSION 2.8)
project(layermgr)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets)

set(LAYERMGR_SRCS
    active_layers_widget.cpp
    layer_locations_widget.cpp
    layer_manager.cpp
    layer_manifest.cpp
    layer_settings_widget.cpp
    override_settings.cpp
    data.qrc
)

add_executable(vklayermanager ${LAYERMGR_SRCS})
target_link_libraries(vklayermanager Qt5::Core Qt5::Gui Qt5::Widgets)
