#include <filesystem>
#include <gtest/gtest.h>
#include "../notifications/Notifier.h"
#include "../notifications/CriticalNotificationTelegram.h"
#include "../notifications/CriticalNotificationEmail.h"
#include "Constants.h"

TEST(Notification, CriticalNotificationTelegram) {
    s21::Notifier notifier;
    s21::CriticalNotificationTelegram tg_nf;
    notifier.setNotification(&tg_nf);

    EXPECT_FALSE(tg_nf.getHostname().empty());
    EXPECT_NO_THROW(notifier.sendNotification(Constants::telegram_id, {"TEST-MSG1", "TEST-MSG2"}));
}

TEST(Notification, CriticalNotificationEmail) {
    s21::Notifier notifier;
    s21::CriticalNotificationEmail email_nf;
    notifier.setNotification(&email_nf);
    
    EXPECT_FALSE(email_nf.getHostname().empty());
    EXPECT_NO_THROW(notifier.sendNotification(Constants::email, {"TEST-MSG1", "TEST-MSG2"}));
}
