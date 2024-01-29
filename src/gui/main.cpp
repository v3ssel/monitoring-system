#include <QApplication>
#include "MonitoringSystemWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    s21::MonitoringSystemWindow w;
    w.show();

    return a.exec();
}
