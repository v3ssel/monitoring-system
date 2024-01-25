#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->agents_to_logs_splitter->setSizes({130, 670});
    ui->logs_to_errs_splitter->setSizes({430, 170});

    connect(ui->start_pushButton, &QAbstractButton::clicked, this, [this]() {
        QPropertyAnimation *a = fadeOut(ui->start_pushButton, 350);

        connect(a, &QPropertyAnimation::finished, this, [this]() {
            ui->stackedWidget->setCurrentIndex(1);

            fadeIn(ui->groupBox, 350);
        });
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

QPropertyAnimation* MainWindow::fadeOut(QWidget *widget, int msec) {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    widget->setGraphicsEffect(eff);

    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(msec);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);

    a->start(QPropertyAnimation::DeleteWhenStopped);

    return a;
}

void MainWindow::fadeIn(QWidget *widget, int msec) {
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    widget->setGraphicsEffect(eff);

    QPropertyAnimation *a2 = new QPropertyAnimation(eff,"opacity");
    a2->setDuration(msec);
    a2->setStartValue(0);
    a2->setEndValue(1);
    a2->setEasingCurve(QEasingCurve::InBack);

    a2->start(QPropertyAnimation::DeleteWhenStopped);
}

