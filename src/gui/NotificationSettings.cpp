#include "NotificationSettings.h"
#include "ui_NotificationSettings.h"

namespace s21 {
NotificationSettings::NotificationSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NotificationSettings) {
    ui->setupUi(this);

    this->setWindowTitle("Notification Settings");
    this->setModal(true);

    batch_size = 5;
    email_enabled = false;
    tg_enabled = false;

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NotificationSettings::buttonBoxAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NotificationSettings::buttonBoxRejected);
}

NotificationSettings::~NotificationSettings() {
    delete ui;
}

void NotificationSettings::buttonBoxAccepted() {
    email_enabled = ui->email_checkBox->isChecked();
    email_address = ui->email_lineEdit->text();

    tg_enabled = ui->tg_checkBox->isChecked();
    tg_id = ui->tg_lineEdit->text();

    batch_size = ui->batch_size_spinBox->value();
}

void NotificationSettings::buttonBoxRejected() {
    ui->email_checkBox->setChecked(email_enabled);
    ui->tg_checkBox->setChecked(tg_enabled);

    ui->email_lineEdit->setText(email_address);
    ui->tg_lineEdit->setText(tg_id);

    ui->batch_size_spinBox->setValue(batch_size);
}

} // namespace s21
