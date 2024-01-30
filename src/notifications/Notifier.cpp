#include "Notifier.h"

namespace s21 {
    void Notifier::setNotification(std::unique_ptr<CriticalNotification> &notification) {
        notification_ = std::move(notification);
    }
    
    void Notifier::sendNotification(const std::string &receiver, const std::vector<std::string>& msgs) {
        notification_->sendNotification(receiver, msgs);
    }
}