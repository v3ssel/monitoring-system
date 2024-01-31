cmake_minimum_required(VERSION 3.5)
project(MonitoringNotifications VERSION 0.1 LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -Wextra)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(NOTIFICATION_SOURCES
    CriticalNotification.cpp
    CriticalNotification.h
    CriticalNotificationEmail.cpp
    CriticalNotificationEmail.h
    CriticalNotificationTelegram.cpp
    CriticalNotificationTelegram.h
    Notifier.cpp
    Notifier.h
)

add_library(${PROJECT_NAME} STATIC 
    ${NOTIFICATION_SOURCES}
)
