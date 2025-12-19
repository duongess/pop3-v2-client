#ifndef _CLI_H_
#define _CLI_H_

#include <string>

class CmdLineInterface;

typedef void (CmdLineInterface::*CLI_CMD_FUNC)(std::string[], int);

#define CLI_CAST(p_func_type) static_cast<CLI_CMD_FUNC>(p_func_type)


const unsigned short CMD_MAX_NUM = 64;
const unsigned short CMD_MAX_ARG_NUM = 16;

const unsigned short CMD_NOOP_ID  =  CMD_MAX_NUM +1;
const unsigned short CMD_UNKNOWN_ID = CMD_MAX_NUM;
const unsigned short CMD_QUIT_ID = CMD_MAX_NUM + 3;

const std::string CMD_QUIT_NAME = "quit";



class CmdLineInterface
{

private:
    std::string  cmdPrompt;
    //uint8_t cmdIdList[CMD_MAX_NUM];
    CLI_CMD_FUNC cmdDoFunc[CMD_MAX_NUM];
    bool bRunning;
    unsigned short numCommands;
    std::string  cmdNameList[CMD_MAX_NUM];
protected:
    unsigned int cmdDefaulID;

public:
    CmdLineInterface();
    CmdLineInterface(const std::string& prompt);
    void setCmdPrompt(const std::string& prompt);
    void clearScreen();
    void showCmdPrompt();
    void addCmd(const std::string& name, CLI_CMD_FUNC f);
    void run(); // run without initial arguments
    void run(char* initArgv[], int initArgc); // run with initial arguments
    void runRailway(char* initArgv[], int initArgc);
protected:
    virtual void initCmd();
    virtual void initConsole();
private:
    void readCmd(std::string& cmd);
    unsigned short parseCmd(const std::string& cmd, std::string cmd_argv[], int& cmd_argc);
    void doUnknown();
    void doQuit();
    void doCmd(unsigned short id, std::string cmd_argv[], int cmd_argc);
};

#endif // _CLI
