#ifndef _COMMANDCALLER_H_
#define _COMMANDCALLER_H_

#include <string>

class CommandCaller {
  public:
    static CommandCaller& getInstance();
    CommandCaller(const CommandCaller &cmd_caller) = delete;
    CommandCaller& operator=(const CommandCaller &cmd_caller) = delete;
    
    std::string takeValue(const std::string& command); 

  private:
    CommandCaller() = default;
};

#endif  // _COMMANDCALLER_H_
