#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <string>

namespace s21 {
    class Observer {
       public:
        virtual ~Observer() = default;
        virtual void NotifyError(const std::string& message) = 0;
    };
}

#endif  // _OBSERVER_H_
