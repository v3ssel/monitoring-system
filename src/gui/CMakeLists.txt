cmake_minimum_required(VERSION 3.5)

project(MonitoringSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        MonitoringSystemWindow.cpp
        MonitoringSystemWindow.h
        MonitoringSystemWindow.ui
        NotificationSettings.cpp
        NotificationSettings.h
        NotificationSettings.ui
        ../controller/KernelController.cpp
        ../controller/KernelController.h
)

set(BUILD_KERNEL ON)
add_subdirectory(../core ${CMAKE_BINARY_DIR}/core)
add_subdirectory(../notifications ${CMAKE_BINARY_DIR}/notifications)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MonitoringSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(MonitoringSystem
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(MonitoringSystem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    MonitoringKernel
    MonitoringNotifications
)

set_target_properties(MonitoringSystem PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install(TARGETS MonitoringSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MonitoringSystem)
endif()
