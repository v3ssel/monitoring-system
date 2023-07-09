#ifndef __CMDCALLER_H__
#define __CMDCALLER_H__
#include <fstream>

class CommandCaller {
    public:
     static CommandCaller& getInstance();
     std::string takeValue(std::string command); 

   private:
    CommandCaller() = default;
    CommandCaller(CommandCaller &cmd_caller);
    CommandCaller(CommandCaller &&cmd_caller);
};

#endif
