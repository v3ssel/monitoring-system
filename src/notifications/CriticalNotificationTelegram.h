#ifndef _CRITICALNOTIFICATIONTELEGRAM_H_
#define _CRITICALNOTIFICATIONTELEGRAM_H_

#include "CriticalNotification.h"

namespace s21 {
    class CriticalNotificationTelegram : public CriticalNotification {
       public:
        void sendNotification(const std::string& receiver, const std::vector<std::string>& msgs) override;
       
       private:
        static const std::string token_;

        std::string urlEncode(const std::string& str) const;
    };
} // namespace s21


#endif  // _CRITICALNOTIFICATIONTELEGRAM_H_
