#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include <memory>
#include <vector>
#include "CriticalNotification.h"

namespace s21 {
    class Notifier {
       public:
        void setNotification(CriticalNotification* notification);
        void sendNotification(const std::string& receiver, const std::vector<std::string>& msgs);

       private:
        CriticalNotification* notification_ = nullptr;
    };
} // namespace s21


#endif  // _NOTIFIER_H_
