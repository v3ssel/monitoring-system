#ifndef _COMMANDCALLER_H_
#define _COMMANDCALLER_H_
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

#endif  // _COMMANDCALLER_H_
