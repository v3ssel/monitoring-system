#include "Notifier.h"

namespace s21 {
    void Notifier::setNotification(CriticalNotification* notification) {
        notification_ = notification;
    }
    
    void Notifier::sendNotification(const std::string &receiver, const std::vector<std::string>& msgs) {
        notification_->sendNotification(receiver, msgs);
    }
}