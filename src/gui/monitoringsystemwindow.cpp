#include "monitoringsystemwindow.h"
#include "./ui_monitoringsystemwindow.h"

#include <QGraphicsOpacityEffect>
#include <QDir>
#include <QThread>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
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

    dateformat_ = "yy-MM-dd";
    update_agents_sec_ = 2;
    skipping_lines_ = 0;

    is_active_cb_ = nullptr;
    agent_name_le_ = nullptr;
    agent_type_le_ = nullptr;
    agent_upd_time_sb_ = nullptr;
    fs_watcher_ = nullptr;

    ui->stackedWidget->setCurrentIndex(0);

    ui->agents_to_logs_splitter->setSizes({130, 670});
    ui->logs_to_errs_splitter->setSizes({430, 170});

    connect(ui->start_pushButton, &QAbstractButton::clicked, this, &MonitoringSystemWindow::startKernel);
    connect(ui->agents_listWidget, &QListWidget::itemClicked, this, &MonitoringSystemWindow::updateAgentInfo);
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

    agents_updater_ = QThread::create(&MonitoringSystemWindow::updateAgentsList, this);
    agents_updater_->start();

    errors_updater_ = QThread::create(&MonitoringSystemWindow::updateErrorsList, this);
    errors_updater_->start();

    criticals_updater_ = QThread::create(&MonitoringSystemWindow::updateCriticalsList, this);
    criticals_updater_->start();

    setupFilewatcher();
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

void MonitoringSystemWindow::updateLogsList(const QString str) {
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

    if (datestr_ != QDateTime::currentDateTime().toString(dateformat_)) {
        delete fs_watcher_;
        fs_watcher_ = nullptr;
        skipping_lines_ = 0;
        setupFilewatcher();
    }
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

void MonitoringSystemWindow::isActiveCheckboxClicked(bool checked) {
    if (checked) {
        KernelController::getInstance().enableAgent(last_clicked_agent_.toStdString());
    } else {
        KernelController::getInstance().disableAgent(last_clicked_agent_.toStdString());
    }
}

void MonitoringSystemWindow::agentNameLineEditEdited() {
    replaceConfigFile("name", agent_name_le_->text());
}

void MonitoringSystemWindow::agentTypeLineEditEdited() {
    replaceConfigFile("type", agent_type_le_->text());
}

void MonitoringSystemWindow::agentUpdateTimeChanged(int new_val) {
    replaceConfigFile("update_time", QString::number(new_val));
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
    addAgentNameInfo(frame, ag->name);
    addAgentTypeInfo(frame, ag->type);
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

void MonitoringSystemWindow::addAgentNameInfo(QFrame *frame, std::string& name) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    QLabel* label = new QLabel("Name:", widget);
    agent_name_le_ = new QLineEdit(QString::fromStdString(name), widget);

    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
    hbox->addWidget(agent_name_le_, 0, Qt::AlignmentFlag::AlignRight);
    frame->layout()->addWidget(widget);

    connect(agent_name_le_, &QLineEdit::editingFinished, this, &MonitoringSystemWindow::agentNameLineEditEdited);
}

void MonitoringSystemWindow::addAgentTypeInfo(QFrame *frame, std::string &type) {
    QWidget* widget = new QWidget(frame);
    QHBoxLayout* hbox = new QHBoxLayout(widget);

    QLabel* label = new QLabel("Type:", widget);
    agent_type_le_ = new QLineEdit(QString::fromStdString(type), widget);

    hbox->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft);
    hbox->addWidget(agent_type_le_, 0, Qt::AlignmentFlag::AlignRight);
    frame->layout()->addWidget(widget);

    connect(agent_type_le_, &QLineEdit::editingFinished, this, &MonitoringSystemWindow::agentTypeLineEditEdited);
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

    for (auto& metric : ag->metrics_names_) {
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

void MonitoringSystemWindow::setupFilewatcher() {
    QDateTime date = QDateTime::currentDateTime();
    datestr_ = date.toString(dateformat_);

    fs_watcher_ = new QFileSystemWatcher(this);
    fs_watcher_->addPath(logs_directory_ + "/" + datestr_ + ".txt");

    connect(fs_watcher_, &QFileSystemWatcher::fileChanged, this, &MonitoringSystemWindow::updateLogsList);
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
    disconnect(agent_name_le_);
    disconnect(agent_type_le_);
    disconnect(agent_upd_time_sb_);

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
    QFile file(configs_directory_ + "/" + QString::fromStdString(agent->config_name));
    file.open(QFile::OpenModeFlag::ReadOnly);

    QStringList file_contents;
    bool changed = false;

    QTextStream ts(&file);
    while (!ts.atEnd()) {
        QString line = ts.readLine();
        if (!changed && line.indexOf(param + ":", 0, Qt::CaseInsensitive) == 0) {
            line.replace(line.indexOf(':') + 1, line.length(), new_val);
            changed = true;
        }
        file_contents.push_back(line);
    }
    file.close();

    if (!changed) {
        file_contents.push_back(param + ":" + new_val);
    }

    file.open(QFile::OpenModeFlag::WriteOnly);
    for (auto& str : file_contents) {
        ts << str << "\n";
    }
}
}

