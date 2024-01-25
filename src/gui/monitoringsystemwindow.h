#ifndef _MONITORINGSYSTEMWINDOW_H_
#define _MONITORINGSYSTEMWINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MonitoringSystemWindow; }
QT_END_NAMESPACE

namespace s21 {
    class MonitoringSystemWindow : public QMainWindow {
        Q_OBJECT

       public:
        MonitoringSystemWindow(QWidget *parent = nullptr);
        ~MonitoringSystemWindow();

       public slots:
        void startKernel();

        void updateAgentsList();
        void updateLogsList();
        void updateCriticalsList();
        void updateErrorsList();

       private:
        Ui::MonitoringSystemWindow *ui;
        QString agents_directory_, configs_directory_, logs_directory_;
        int update_agents_sec_;

        QThread *agents_updater_, *errors_updater_, *criticals_updater_, *logs_updater_;

        QPropertyAnimation* fadeOut(QWidget* widget, int msec);
        void fadeIn(QWidget* widget, int msec);
    };
}

#endif // _MONITORINGSYSTEMWINDOW_H_
