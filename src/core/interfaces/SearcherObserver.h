#ifndef _SEARCHEROBSERVER_H_
#define _SEARCHEROBSERVER_H_

#include <string>
#include "Observer.h"

namespace s21 {
    class SearcherObserver : public s21::Observer {
       public:
        virtual void NotifyNewAgentLoaded(const std::string& agent_name) = 0;
    };
}

#endif  // _SEARCHEROBSERVER_H_
