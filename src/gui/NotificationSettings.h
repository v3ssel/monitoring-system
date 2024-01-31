#ifndef _NOTIFICATIONSETTINGS_H_
#define _NOTIFICATIONSETTINGS_H_

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class NotificationSettings; }
QT_END_NAMESPACE

namespace s21 {
class NotificationSettings : public QDialog {
    Q_OBJECT

   public:
    NotificationSettings(QWidget *parent = nullptr);
    ~NotificationSettings();

    QString email_address, tg_id;
    bool email_enabled, tg_enabled;
    int batch_size;

   public slots:
    void buttonBoxAccepted();
    void buttonBoxRejected();

   private:
    Ui::NotificationSettings* ui;
};

} // namespace s21

#endif // _NOTIFICATIONSETTINGS_H_
