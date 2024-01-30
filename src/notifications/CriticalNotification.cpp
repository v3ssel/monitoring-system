#include "CriticalNotification.h"

#if defined __WIN32__
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace s21 {
    std::string CriticalNotification::getHostname() {
        std::string hostname;

        #if defined __WIN32__
            DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
            TCHAR host_win[MAX_COMPUTERNAME_LENGTH + 1];
            GetComputerName(host_win, &size);
            
            hostname = host_win;
        #else
            char host_nx[HOST_NAME_MAX];
            gethostname(host_nx, HOST_NAME_MAX);

            hostname = host_nx;
        #endif

        return hostname;
    }
}
