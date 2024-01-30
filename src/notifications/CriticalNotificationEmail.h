#ifndef _CRITICALNOTIFICATIONEMAIL_H_
#define _CRITICALNOTIFICATIONEMAIL_H_

#include "CriticalNotification.h"

namespace s21 {
    class CriticalNotificationEmail : public CriticalNotification {
       public:
        void sendNotification(const std::string& receiver, const std::vector<std::string>& msgs) override;
    
       private:
        static const std::string sender_mail_;
        static const std::string sender_password_;
    };
}

#endif  // _CRITICALNOTIFICATIONEMAIL_H_
