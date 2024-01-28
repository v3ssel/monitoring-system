#ifndef _MONITORINGSYSTEMWINDOW_H_
#define _MONITORINGSYSTEMWINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemWatcher>
#include <QPropertyAnimation>
#include <QListWidgetItem>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include "../core/utils/CompareType.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MonitoringSystemWindow; }
QT_END_NAMESPACE

namespace s21 {
    class Agent;

    class MonitoringSystemWindow : public QMainWindow {
        Q_OBJECT

       public:
        MonitoringSystemWindow(QWidget *parent = nullptr);
        ~MonitoringSystemWindow();

       public slots:
        void startKernel();

        void updateAgentsList();
        void updateAgentInfo(QListWidgetItem* item);
        void updateLogsList(const QString str);
        void updateCriticalsList();
        void updateErrorsList();

        void isActiveCheckboxClicked(bool checked);
        void agentNameLineEditEdited();
        void agentTypeLineEditEdited();
        void agentUpdateTimeChanged(int new_val);

       private:
        Ui::MonitoringSystemWindow *ui;

        QString datestr_, dateformat_, last_clicked_agent_;
        QString agents_directory_, configs_directory_, logs_directory_;
        int update_agents_sec_;

        QThread *agents_updater_, *errors_updater_, *criticals_updater_, *logs_updater_;
        QFileSystemWatcher* fs_watcher_;
        int skipping_lines_;

        QCheckBox* is_active_cb_;
        QLineEdit* agent_name_le_, *agent_type_le_;
        QSpinBox* agent_upd_time_sb_;

        QPropertyAnimation* fadeOut(QWidget* widget, int msec);
        void fadeIn(QWidget* widget, int msec);

        void setupFilewatcher();
        QComboBox* getComparesComboBox();
        int parseCompareFnc(CompareType fnc);
        void clearWidget(QWidget* widget);
        void replaceConfigFile(const QString& param, const QString& new_val);

        void addAgentIsActiveInfo(QFrame* frame, bool is_active);
        void addAgentNameInfo(QFrame* frame, std::string& name);
        void addAgentTypeInfo(QFrame* frame, std::string& type);
        void addAgentUpdTimeInfo(QFrame* frame, int update_time);
        void addAgentActiveTimeInfo(QFrame* frame, QFont& font, std::string& agent_name);
        void addAgentMetricsInfo(QFrame* frame, QFont& font, std::shared_ptr<Agent>& ag);
    };
}

#endif // _MONITORINGSYSTEMWINDOW_H_
