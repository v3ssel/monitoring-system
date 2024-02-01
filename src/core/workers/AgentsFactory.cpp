#if defined __APPLE__
    #include <dlfcn.h>
#elif defined __WIN32__
    #include <windows.h>
#endif

#include <stdexcept>
#include "AgentsFactory.h"

namespace s21 {
    AgentsFactory &AgentsFactory::GetInstance() {
        static AgentsFactory instance;

        return instance;
    }

    std::shared_ptr<Agent> AgentsFactory::LoadAgent(const std::string& library) {
        std::function<Agent*()> fcreate;
        
        #if defined __APPLE__
            void *library_handler = dlopen(library.c_str(), RTLD_LAZY);
            if (!library_handler) throw std::runtime_error(dlerror());

            fcreate = reinterpret_cast<Agent*(*)()>(dlsym(library_handler, "create_obj"));
            if (dlerror()) throw std::runtime_error(dlerror());

        #elif defined __WIN32__
            HINSTANCE library_handler = LoadLibrary(library.c_str());
            if (!library_handler) throw std::runtime_error("Could not load the DLL - " + library + ".");
            
            fcreate = reinterpret_cast<Agent*(*)()>(GetProcAddress(library_handler, "create_obj"));
            if (!fcreate) throw std::runtime_error("Could not locate the create function in DLL - " + library + ".");

        #endif

        return std::shared_ptr<Agent>(fcreate());
    }
}
