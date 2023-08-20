#include "agentsFactory.h"

s21::AgentsFactory::AgentsFactory(std::string library) {
    void *library_handler = dlopen(library.c_str(), RTLD_LAZY);
    if (!library_handler) throw std::runtime_error(dlerror());

    std::function<s21::Agent*()> fcreate = reinterpret_cast<s21::Agent*(*)()>(dlsym(library_handler, "create_obj"));
    if (dlerror()) throw std::runtime_error(dlerror());

    instance_ = std::unique_ptr<s21::Agent>(fcreate());
}

std::unique_ptr<s21::Agent>& s21::AgentsFactory::GetAgent() {
    return instance_;
}
