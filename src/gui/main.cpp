#include <QApplication>
#include "MonitoringSystemWindow.h"

#if defined __WIN32__
    #include <windows.h>
    #include <cstdio>
#endif

int main(int argc, char *argv[]) {
    #if defined __WIN32__
        FreeConsole();
        AllocConsole();
        AttachConsole(GetCurrentProcessId());

        std::freopen("CON", "w", stdout);
        std::freopen("CON", "w", stderr);
        std::freopen("CON", "r", stdin);
    #endif

    QApplication a(argc, argv);
    s21::MonitoringSystemWindow w;
    w.show();

    return a.exec();
}
