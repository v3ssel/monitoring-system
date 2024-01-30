#ifndef _CRITICALNOTIFICATION_H_
#define _CRITICALNOTIFICATION_H_

#include <string>
#include <vector>

namespace s21 {
    class CriticalNotification {
       public:
        virtual void sendNotification(const std::string& receiver, const std::vector<std::string>& msgs) = 0;
        virtual ~CriticalNotification() = default;

        std::string getHostname();
    };
}

#endif  // _CRITICALNOTIFICATION_H_
