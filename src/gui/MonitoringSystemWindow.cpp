#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QDir>
#include <QChar>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QLineEdit>
#include "../controller/KernelController.h"
#include "../core/agents/AgentConfigWriter.h"

#include "MonitoringSystemWindow.h"
#include "./ui_MonitoringSystemWindow.h"

namespace s21 {
MonitoringSystemWindow::MonitoringSystemWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MonitoringSystemWindow) {
    nt_set_ = new NotificationSettings(this);
    ui->setupUi(this);

    this->setWindowTitle("Monitoring System");
    setupWindow();

    agents_updater_ = nullptr;
    errors_updater_ = nullptr;
    criticals_updater_ = nullptr;

    ui->stackedWidget->setCurrentIndex(0);

    ui->agents_to_logs_splitter->setSizes({270, 900});
    ui->logs_to_errs_splitter->setSizes({500, 200});

    connect(ui->start_pushButton, &QAbstractButton::clicked, this, &MonitoringSystemWindow::startKernel);
    connect(ui->agents_listWidget, &QListWidget::itemClicked, this, &MonitoringSystemWindow::updateAgentInfo);

    setupActions();
}

MonitoringSystemWindow::~MonitoringSystemWindow() {
    KernelController::getInstance().deleteKernel();

    delete ui;
    ui = nullptr;
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

    if (!agents_updater_) {
        agents_updater_ = QThread::create(&MonitoringSystemWindow::updateAgentsList, this);
        agents_updater_->start();
    }

    if (!errors_updater_) {
        errors_updater_ = QThread::create(&MonitoringSystemWindow::updateErrorsList, this);
        errors_updater_->start();
    }

    if (!criticals_updater_) {
        criticals_updater_ = QThread::create(&MonitoringSystemWindow::updateCriticalsList, this);
        criticals_updater_->start();
    }

    setupFilewatcher();
}

void MonitoringSystemWindow::stopKernel() {
    KernelController::getInstance().stopKernel();
    clearAll();
    setupWindow();

    QPropertyAnimation* anim = fadeOut(ui->groupBox, 350);

    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);

        fadeIn(ui->start_pushButton, 350);
    });
}

void MonitoringSystemWindow::updateAgentsList() {
    while (true) {
        std::set<std::string> agents = KernelController::getInstance().getActiveAgents();
        for (auto& agent : agents) {
            if (!ui) return;

            if (ui->agents_listWidget->findItems(QString::fromStdString(agent), Qt::MatchFlag::MatchExactly).empty()) {
                ui->agents_listWidget->addItem(QString::fromStdString(agent));
            }
        }

        QThread::sleep(update_agents_sec_);
    }
}

void MonitoringSystemWindow::updateLogsList(const QString str) {
    QFile f(str);
    if (!f.exists()) {
        reloadFilewatcher();
    }
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

    if (datestr_ != QDateTime::currentDateTime().toString(dateformat_)) {
        reloadFilewatcher();
    }
}

void MonitoringSystemWindow::updateErrorsList() {
    while (true) {
        std::queue<std::string> errs = KernelController::getInstance().takeErrors();
        std::vector<std::string> errors_vec;
        errors_vec.reserve(errs.size());

        while (!errs.empty()) {
            if (!ui) return;

            ui->errors_listWidget->insertItem(0, QString::fromStdString(errs.front()));
            errors_vec.push_back(errs.front());
            errs.pop();
        }

        QThread::sleep(1);
    }
}

void MonitoringSystemWindow::updateCriticalsList() {
    std::vector<std::string> crits_vec;
    crits_vec.reserve(nt_set_->batch_size);

    while (true) {
        std::queue<std::string> crits = KernelController::getInstance().takeCriticals();

        while (!crits.empty()) {
            if (!ui) return;

            ui->criticals_listWidget->insertItem(0, QString::fromStdString(crits.front()));
            crits_vec.push_back(crits.front());
            crits.pop();
        }

        if (crits_vec.size() >= nt_set_->batch_size) {
            if (nt_set_->tg_enabled) {
                QFuture<void> f = QtConcurrent::run([crits_vec, tg_id = nt_set_->tg_id.toStdString()]() {
                    KernelController::getInstance().sendNotificationToTelegram(tg_id, crits_vec);
                });
            }

            if (nt_set_->email_enabled) {
                QFuture<void> f = QtConcurrent::run([crits_vec, email_address = nt_set_->email_address.toStdString()]() {
                    KernelController::getInstance().sendNotificationToEmail(email_address, crits_vec);
                });
            }

            crits_vec.clear();
        }

        QThread::sleep(1);
    }
}

void MonitoringSystemWindow::clearCriticalsList() {
    ui->criticals_listWidget->clear();
}

void MonitoringSystemWindow::changeSearchDirectory() {
    QString new_dir = QFileDialog::getExistingDirectory(this, "Select new search directory", QDir::currentPath());
    if (new_dir.isNull()) return;
    agents_directory_ = new_dir;

    KernelController::getInstance().changeSearchDirectory(agents_directory_.toStdString());
}

void MonitoringSystemWindow::changeConfigsDirectory() {
    QString new_dir = QFileDialog::getExistingDirectory(this, "Select new configs directory", QDir::currentPath());
    if (new_dir.isNull()) return;
    configs_directory_ = new_dir;

    KernelController::getInstance().changeConfigsDirectory(configs_directory_.toStdString());
}

void MonitoringSystemWindow::changeLogsDirectory() {
    QString new_dir = QFileDialog::getExistingDirectory(this, "Select new logs directory", QDir::currentPath());
    if (new_dir.isNull()) return;
    logs_directory_ = new_dir;

    KernelController::getInstance().changeLogsDirectory(logs_directory_.toStdString());
    reloadFilewatcher();
}

void MonitoringSystemWindow::startLogsWriter() {
    start_writer_act_->setEnabled(false);
    KernelController::getInstance().startWriter();
    stop_writer_act_->setEnabled(true);
}

void MonitoringSystemWindow::stopLogsWriter() {
    stop_writer_act_->setEnabled(false);
    KernelController::getInstance().stopWriter();
    start_writer_act_->setEnabled(true);
}

void MonitoringSystemWindow::startAgentsSearcher() {
    start_searcher_act_->setEnabled(false);
    KernelController::getInstance().startSearcher();
    stop_searcher_act_->setEnabled(true);
}

void MonitoringSystemWindow::stopAgentsSearcher() {
    stop_searcher_act_->setEnabled(false);
    KernelController::getInstance().stopSearcher();
    start_searcher_act_->setEnabled(true);
}

void MonitoringSystemWindow::clearErrorsList() {
    ui->errors_listWidget->clear();
}

void MonitoringSystemWindow::isActiveCheckboxClicked(bool checked) {
    if (checked) {
        KernelController::getInstance().enableAgent(last_clicked_agent_.toStdString());
    } else {
        KernelController::getInstance().disableAgent(last_clicked_agent_.toStdString());
    }
}

void MonitoringSystemWindow::agentConfLineEdited() {
    QLineEdit* param_value = qobject_cast<QLineEdit*>(sender());
    QLabel* param_name = agent_confdata_to_name_[param_value];

    QString pname_qstr = param_name->text();
    pname_qstr.front() = pname_qstr.front().toLower();
    pname_qstr.chop(1);

    replaceConfigFile(pname_qstr, param_value->text());
}

void MonitoringSystemWindow::agentUpdateTimeChanged(int new_val) {
    replaceConfigFile("update_time", QString::number(new_val));
}

void MonitoringSystemWindow::agentMetricCompareChanged(const QString &str) {
    QComboBox* cbox = qobject_cast<QComboBox*>(sender());
    QLabel* metric_name = metric_cmp_to_name_[cbox];

    auto& agent = KernelController::getInstance().getAgentByName(last_clicked_agent_.toStdString());
    QString filename = configs_directory_ + "/" + QString::fromStdString(agent->config_name);

    KernelController::getInstance().writeCompareSignToConfig(filename.toStdString(), metric_name->text().toStdString(), str.toStdString());
}

void MonitoringSystemWindow::agentMetricCriticalEdited(const QString &str) {
    QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
    QLabel* metric_name = metric_crit_to_name_[ledit];

    auto& agent = KernelController::getInstance().getAgentByName(last_clicked_agent_.toStdString());
    QString filename = configs_directory_ + "/" + QString::fromStdString(agent->config_name);

    AgentConfigWriter::write(filename.toStdString(), metric_name->text().toStdString(), "", str.toStdString());
    KernelController::getInstance().writeCriticalToConfig(filename.toStdString(), metric_name->text().toStdString(), str.toStdString());
}

void MonitoringSystemWindow::showNotificationSettingsDialog() {
    nt_set_->show();
}

void MonitoringSystemWindow::updateAgentInfo(QListWidgetItem* item) {
    clearWidget(ui->agents_info_frame);

    last_clicked_agent_ = item->text();
    std::string agent = last_clicked_agent_.toStdString();
    std::shared_ptr<Agent> ag = KernelController::getInstance().getAgentByName(agent);

    QFrame* frame = ui->agents_info_frame;
    frame->layout()->setSpacing(0);

    QFont font;
    font.setBold(true);
    font.setPointSize(10);

    addAgentIsActiveInfo(frame, ag->is_active);
    addAgentConfLines(frame, ag);
    addAgentUpdTimeInfo(frame, ag->getUpdateTime());
    addAgentActiveTimeInfo(frame, font, agent);
    addAgentMetricsInfo(frame, font, ag);

    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->agents_info_frame->layout()->addItem(spacer);
}

void MonitoringSystemWindow::addAgentIsActiveInfo(QFrame *frame, bool is_active) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    QLabel* label = new QLabel("Is Active:", widget);
    is_active_cb_ = new QCheckBox(widget);
    is_active_cb_->setChecked(is_active);

    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
    hbox->addWidget(is_active_cb_, 0, Qt::AlignmentFlag::AlignRight);
    frame->layout()->addWidget(widget);

    connect(is_active_cb_, &QCheckBox::clicked, this, &MonitoringSystemWindow::isActiveCheckboxClicked);
}

void MonitoringSystemWindow::addAgentConfLines(QFrame *frame, std::shared_ptr<Agent>& ag) {
    QWidget* widget = nullptr;
    QHBoxLayout* hbox = nullptr;

    QLabel* label = new QLabel("Name:", widget);
    QLineEdit* conf_le = new QLineEdit(QString::fromStdString(ag->name), widget);

    auto addWidget = [&]() {
        widget = new QWidget(frame);
        hbox = new QHBoxLayout(widget);

        hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
        hbox->addWidget(conf_le, 0, Qt::AlignmentFlag::AlignRight);
        frame->layout()->addWidget(widget);

        connect(conf_le, &QLineEdit::editingFinished, this, &MonitoringSystemWindow::agentConfLineEdited);
        agent_confdata_to_name_[conf_le] = label;
    };
    addWidget();

    label = new QLabel("Type:", widget);
    conf_le = new QLineEdit(QString::fromStdString(ag->type), widget);
    addWidget();

    for (auto& [param_name, value] : ag->additional_params) {
        QString pname_qstr = QString::fromStdString(param_name) + ":";
        pname_qstr.front() = pname_qstr.front().toUpper();

        label = new QLabel(pname_qstr, widget);
        conf_le = new QLineEdit(QString::fromStdString(value), widget);
        addWidget();
    }
}

void MonitoringSystemWindow::addAgentUpdTimeInfo(QFrame *frame, int update_time) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    QLabel* label = new QLabel("Seconds Update Time:", widget);
    agent_upd_time_sb_ = new QSpinBox(widget);
    agent_upd_time_sb_->setValue(update_time);
    agent_upd_time_sb_->setSingleStep(1);
    agent_upd_time_sb_->setMinimum(1);
    agent_upd_time_sb_->setMaximum(60);

    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
    hbox->addWidget(agent_upd_time_sb_, 0, Qt::AlignmentFlag::AlignRight);
    frame->layout()->addWidget(widget);

    connect(agent_upd_time_sb_, &QSpinBox::valueChanged, this, &MonitoringSystemWindow::agentUpdateTimeChanged);
}

void MonitoringSystemWindow::addAgentActiveTimeInfo(QFrame *frame, QFont& font, std::string& agent_name) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    double active_time_ms = KernelController::getInstance().getAgentActiveTime(agent_name).count() / 1000.0;
    QLabel* label = new QLabel("Seconds Active Time:", widget);
    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);

    label = new QLabel(QString::number(active_time_ms, 'f', 2));
    label->setFont(font);
    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignRight);

    frame->layout()->addWidget(widget);
}

void MonitoringSystemWindow::addAgentMetricsInfo(QFrame *frame, QFont& font, std::shared_ptr<Agent>& ag) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    QLabel* label = new QLabel("Metrics", widget);
    label->setFont(font);

    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignCenter);
    frame->layout()->addWidget(widget);

    for (auto& metric : ag->getMetricNames()) {
        widget = new QWidget(frame);
        hbox = new QHBoxLayout(widget);

        label = new QLabel(QString::fromStdString(metric), widget);
        QComboBox* combobox = getComparesComboBox();
        combobox->setParent(widget);
        combobox->setCurrentIndex(parseCompareFnc(ag->getCompareFncType(metric)));
        QLineEdit* le = new QLineEdit(QString::number(ag->getCriticalValue(metric)), widget);

        hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
        hbox->addWidget(combobox, 0, Qt::AlignmentFlag::AlignCenter);
        hbox->addWidget(le, 0, Qt::AlignmentFlag::AlignRight);
        frame->layout()->addWidget(widget);

        connect(combobox, &QComboBox::currentTextChanged, this, &MonitoringSystemWindow::agentMetricCompareChanged);
        connect(le, &QLineEdit::textEdited, this, &MonitoringSystemWindow::agentMetricCriticalEdited);

        metric_cmp_to_name_[combobox] = label;
        metric_crit_to_name_[le] = label;
    }
}

QPropertyAnimation* MonitoringSystemWindow::fadeOut(QWidget *widget, int msec) {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
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
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    widget->setGraphicsEffect(eff);

    QPropertyAnimation *a2 = new QPropertyAnimation(eff, "opacity");
    a2->setDuration(msec);
    a2->setStartValue(0);
    a2->setEndValue(1);
    a2->setEasingCurve(QEasingCurve::InBack);

    a2->start(QPropertyAnimation::DeleteWhenStopped);
}

void MonitoringSystemWindow::setupWindow() {
    agents_directory_ = QDir::currentPath() + "/agents";
    configs_directory_ = QDir::currentPath() + "/config";
    logs_directory_ = QDir::currentPath() + "/logs";

    dateformat_ = "yy-MM-dd";
    update_agents_sec_ = 2;
    skipping_lines_ = 0;

    is_active_cb_ = nullptr;
    agent_upd_time_sb_ = nullptr;
    fs_watcher_ = nullptr;
}

void MonitoringSystemWindow::setupFilewatcher() {
    QDateTime date = QDateTime::currentDateTime();
    datestr_ = date.toString(dateformat_);
    QString log_file = logs_directory_ + "/" + datestr_ + ".txt";

    QFile file(log_file, this);
    if (!file.exists()) {
        file.open(QFile::OpenModeFlag::ReadWrite);
        file.close();
    }

    fs_watcher_ = new QFileSystemWatcher(this);
    fs_watcher_->addPath(log_file);

    connect(fs_watcher_, &QFileSystemWatcher::fileChanged, this, &MonitoringSystemWindow::updateLogsList);
}

void MonitoringSystemWindow::reloadFilewatcher() {
    delete fs_watcher_;
    fs_watcher_ = nullptr;
    skipping_lines_ = 0;
    setupFilewatcher();
}

void MonitoringSystemWindow::setupActions() {
    QAction* clear_errs_action = new QAction("Clear errors.", this);
    connect(clear_errs_action, &QAction::triggered, this, &MonitoringSystemWindow::clearErrorsList);
    ui->errors_listWidget->addAction(clear_errs_action);

    QAction* clear_crits_action = new QAction("Clear critical values.", this);
    connect(clear_crits_action, &QAction::triggered, this, &MonitoringSystemWindow::clearCriticalsList);
    enable_notifications_ = new QAction("Open notification settings.", this);
    connect(enable_notifications_, &QAction::triggered, this, &MonitoringSystemWindow::showNotificationSettingsDialog);
    ui->criticals_listWidget->addActions({clear_crits_action, enable_notifications_});

    QAction* change_srch_dir_act = new QAction("Change search directory.", this);
    QAction* change_conf_dir_act = new QAction("Change configs directory.", this);
    QAction* change_logs_dir_act = new QAction("Change logs directory.", this);
    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    stop_searcher_act_ = new QAction("Stop agents searcher.", this);
    start_searcher_act_ = new QAction("Start agents searcher.", this);
    start_searcher_act_->setEnabled(false);

    connect(change_srch_dir_act, &QAction::triggered, this, &MonitoringSystemWindow::changeSearchDirectory);
    connect(change_conf_dir_act, &QAction::triggered, this, &MonitoringSystemWindow::changeConfigsDirectory);
    connect(change_logs_dir_act, &QAction::triggered, this, &MonitoringSystemWindow::changeLogsDirectory);
    connect(stop_searcher_act_, &QAction::triggered, this, &MonitoringSystemWindow::stopAgentsSearcher);
    connect(start_searcher_act_, &QAction::triggered, this, &MonitoringSystemWindow::startAgentsSearcher);
    ui->agents_listWidget->addActions({change_srch_dir_act, change_conf_dir_act, change_logs_dir_act, separator, stop_searcher_act_, start_searcher_act_});

    stop_writer_act_ = new QAction("Stop logs writer.", this);
    start_writer_act_ = new QAction("Start logs writer.", this);
    start_writer_act_->setEnabled(false);
    separator = new QAction(this);
    separator->setSeparator(true);
    stop_kernel_act_ = new QAction("STOP KERNEL", this);
    connect(stop_writer_act_, &QAction::triggered, this, &MonitoringSystemWindow::stopLogsWriter);
    connect(start_writer_act_, &QAction::triggered, this, &MonitoringSystemWindow::startLogsWriter);
    connect(stop_kernel_act_, &QAction::triggered, this, &MonitoringSystemWindow::stopKernel);
    ui->logs_listWidget->addActions({stop_writer_act_, start_writer_act_, separator, stop_kernel_act_});
}

QComboBox *MonitoringSystemWindow::getComparesComboBox() {
    QComboBox* cbox = new QComboBox();

    cbox->addItem(">");
    cbox->addItem(">=");
    cbox->addItem("<");
    cbox->addItem("<=");
    cbox->addItem("==");
    cbox->addItem("!=");

    return cbox;
}

int MonitoringSystemWindow::parseCompareFnc(CompareType type) {
    if (type == CompareType::IS_GREATER) {
        return 0;
    }

    if (type == CompareType::IS_GREATER_OR_EQ) {
        return 1;
    }

    if (type == s21::CompareType::IS_LESS) {
        return 2;
    }

    if (type == CompareType::IS_LESS_OR_EQ) {
        return 3;
    }

    if (type == CompareType::IS_EQ) {
        return 4;
    }

    if (type == CompareType::IS_NOT_EQ) {
        return 5;
    }

    qDebug() << -1;
    return -1;
}

void MonitoringSystemWindow::clearWidget(QWidget* widget) {
    disconnect(is_active_cb_);
    disconnect(agent_upd_time_sb_);
    agent_confdata_to_name_.clear();
    metric_cmp_to_name_.clear();
    metric_crit_to_name_.clear();

    if (widget->layout()->count() > 0) {
        QLayoutItem* layout_item;
        while ((layout_item = widget->layout()->takeAt(0))) {
            delete layout_item->widget();
            delete layout_item;
        }
    }
}

void MonitoringSystemWindow::replaceConfigFile(const QString& param, const QString &new_val) {
    auto& agent = KernelController::getInstance().getAgentByName(last_clicked_agent_.toStdString());
    QString filename = configs_directory_ + "/" + QString::fromStdString(agent->config_name);

    KernelController::getInstance().writeParamToConfig(filename.toStdString(), param.toStdString(), new_val.toStdString());
}

void MonitoringSystemWindow::clearAll() {
    clearWidget(ui->agents_info_frame);
    clearCriticalsList();
    clearErrorsList();
    ui->logs_listWidget->clear();
    ui->agents_listWidget->clear();

    last_clicked_agent_ = "";
    if (fs_watcher_) {
        delete fs_watcher_;
        fs_watcher_ = nullptr;
    }
}
}
