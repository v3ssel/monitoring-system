#include "monitoringsystemwindow.h"
#include "./ui_monitoringsystemwindow.h"

#include <QGraphicsOpacityEffect>
#include <QDir>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "../controller/KernelController.h"

namespace s21 {
MonitoringSystemWindow::MonitoringSystemWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MonitoringSystemWindow) {
    ui->setupUi(this);

    this->setWindowTitle("Monitoring System");
    agents_directory_ = QDir::currentPath() + "/agents";
    configs_directory_ = QDir::currentPath() + "/config";
    logs_directory_ = QDir::currentPath() + "/logs";

    update_agents_sec_ = 2;
    skipping_lines_ = 0;

    fs_watcher_ = new QFileSystemWatcher(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->agents_to_logs_splitter->setSizes({130, 670});
    ui->logs_to_errs_splitter->setSizes({430, 170});

    connect(ui->start_pushButton, &QAbstractButton::clicked, this, &MonitoringSystemWindow::startKernel);
    connect(ui->agents_listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        ui->agent_info_listWidget->clear();
        std::string agent = item->text().toStdString();

        std::shared_ptr<Agent> ag = KernelController::getInstance().getAgentByName(agent);
        ui->agent_info_listWidget->addItem("Is Active: " + QString(ag->is_active ? "true" : "false"));
        ui->agent_info_listWidget->addItem(QString::fromStdString("Name: " + ag->name));
        ui->agent_info_listWidget->addItem(QString::fromStdString("Type: " + ag->type));
        ui->agent_info_listWidget->addItem(QString("Update Time: ") + QString::number(ag->getUpdateTime()));
        ui->agent_info_listWidget->addItem(QString("Active Time: ") + QString::number(KernelController::getInstance().getAgentActiveTime(agent).count() / 1000.0) + "ms");
        ui->agent_info_listWidget->addItem("Metrics:");
        for (auto& metric : ag->metrics_names_) {
            ui->agent_info_listWidget->addItem("  " + QString::fromStdString(metric));
        }
        ui->agent_info_listWidget->addItem("");
        ui->agent_info_listWidget->addItem("Disable agent.");
    });
}

MonitoringSystemWindow::~MonitoringSystemWindow() {
    delete ui;

    KernelController::getInstance().deleteKernel();
}

void MonitoringSystemWindow::startKernel() {
    QPropertyAnimation *a = fadeOut(ui->start_pushButton, 350);

    connect(a, &QPropertyAnimation::finished, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);

        fadeIn(ui->groupBox, 350);
    });

    Kernel *k = new Kernel(agents_directory_.toStdString(), logs_directory_.toStdString(), configs_directory_.toStdString(), update_agents_sec_);
    KernelController::getInstance().setKernel(k);
    KernelController::getInstance().startKernel();

//    QtConcurrent::run(updateAgentsList, this);
    agents_updater_ = QThread::create(&MonitoringSystemWindow::updateAgentsList, this);
    agents_updater_->start();

    errors_updater_ = QThread::create(&MonitoringSystemWindow::updateErrorsList, this);
    errors_updater_->start();

    criticals_updater_ = QThread::create(&MonitoringSystemWindow::updateCriticalsList, this);
    criticals_updater_->start();

    updateLogsList();
}

void MonitoringSystemWindow::updateAgentsList() {
    while (true) {
        std::set<std::string> agents = KernelController::getInstance().getActiveAgents();
        for (auto& agent : agents) {
            if (ui->agents_listWidget->findItems(QString::fromStdString(agent), Qt::MatchFlag::MatchExactly).empty()) {
                ui->agents_listWidget->addItem(QString::fromStdString(agent));
            }
        }

        QThread::sleep(update_agents_sec_);
    }
}

void MonitoringSystemWindow::updateLogsList() {
    QDateTime date = QDateTime::currentDateTime();
    QString datestr = date.toString("yy-MM-dd");

    fs_watcher_->addPath(logs_directory_ + "/" + datestr + ".txt");

    connect(fs_watcher_, &QFileSystemWatcher::fileChanged, this, [this](const QString &str) {
        QFile f(str);
        f.open(QFile::OpenModeFlag::ReadOnly);

        char ch;
        size_t line_cnt = 0;
        while (line_cnt != skipping_lines_ && f.getChar(&ch)) {
            if (ch == '\n')
                line_cnt++;
        }

        QTextStream in(&f);
        while (!in.atEnd()) {
            QString line = in.readLine();
            ui->logs_listWidget->insertItem(0, line);
            if (ui->logs_listWidget->count() > 20) {
                for (size_t i = 21; i < ui->logs_listWidget->count(); i++) {
                    QListWidgetItem* w = ui->logs_listWidget->takeItem(i);
                    if (w) delete w;
                }
            }

            line_cnt++;
        }
        skipping_lines_ = line_cnt;
    });
}

void MonitoringSystemWindow::updateErrorsList() {
    while (true) {
        std::queue<std::string> errs = KernelController::getInstance().takeErrors();
        while (!errs.empty()) {
            ui->errors_listWidget->insertItem(0, QString::fromStdString(errs.front()));
            errs.pop();
        }

        QThread::sleep(1);
    }
}

void MonitoringSystemWindow::updateCriticalsList() {
    while (true) {
        std::queue<std::string> crits = KernelController::getInstance().takeCriticals();
        while (!crits.empty()) {
            ui->criticals_listWidget->insertItem(0, QString::fromStdString(crits.front()));
            crits.pop();
        }

        QThread::sleep(1);
    }
}

QPropertyAnimation* MonitoringSystemWindow::fadeOut(QWidget *widget, int msec) {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this); // delete needed?
    widget->setGraphicsEffect(eff);

    QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
    a->setDuration(msec);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);

    a->start(QPropertyAnimation::DeleteWhenStopped);

    return a;
}

void MonitoringSystemWindow::fadeIn(QWidget *widget, int msec) {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this); // delete needed?
    widget->setGraphicsEffect(eff);

    QPropertyAnimation *a2 = new QPropertyAnimation(eff, "opacity");
    a2->setDuration(msec);
    a2->setStartValue(0);
    a2->setEndValue(1);
    a2->setEasingCurve(QEasingCurve::InBack);

    a2->start(QPropertyAnimation::DeleteWhenStopped);
}
}

