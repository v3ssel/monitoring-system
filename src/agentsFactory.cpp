#include "agentsFactory.h"

s21::AgentsFactory::AgentsFactory(std::string library) {
    void *library_handler = dlopen(library.c_str(), RTLD_LAZY);
    if (!library_handler) throw std::runtime_error(dlerror());

    std::function<s21::Agent*()> fcreate = reinterpret_cast<s21::Agent*(*)()>(dlsym(library_handler, "create_obj"));
    if (dlerror()) throw std::runtime_error(dlerror());

    instance_ = std::unique_ptr<s21::Agent>(fcreate());
}

s21::AgentsFactory::AgentsFactory(AgentsFactory&& af) {
    instance_ = std::move(af.instance_);
    af.instance_ = nullptr;
}

// s21::AgentsFactory& s21::AgentsFactory::operator=(AgentsFactory&& af) {
//     if (&af != this) {
//         instance_ = std::move(af.instance_);
//         af.instance_ = nullptr;
//     }
//     return *this;
// }


std::unique_ptr<s21::Agent>& s21::AgentsFactory::GetAgent() {
    return instance_;
}
