#include "utils.h"
#include "tcpclient.h"
#include "cli.h"
#include "db.h"

class Pop3V2Client : public TcpClient, public CmdLineInterface {
public:
    Pop3V2Client();
    virtual void initCmd();

private:
    std::string hostname;
    std::string username;
    ssize_t accountId;
    void connect(const std::string& serverHost, const std::string& port);

    std::string responseMess();
    std::string getSingleLineResponse(const std::string& mess);
    std::string getMultiLineResponse(const std::string& mess);

    DB db;
    void doLogin(std::string cmd_argv[], int cmd_argc);
    void doLogout(std::string cmd_argv[], int cmd_argc);
    void doSync(std::string cmd_argv[], int cmd_argc);
    void doHelp(std::string cmd_argv[], int cmd_argc);
    void doQuit(std::string cmd_argv[], int cmd_argc);
};