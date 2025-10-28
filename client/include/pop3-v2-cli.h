#ifndef POP3V2_CLIENT_CLI_H // include guard
#define POP3V2_CLIENT_CLI_H

#include "cli.h"
#include "client.h"

class POP3V2ClientCLI: public CmdLineInterface{
    public:
        Client pop3Client;
        POP3V2ClientCLI();
        virtual void initCmd();
    protected:
        std::string hostname;
        std::string user;

    private:
        void doLogin(std::string cmd_argv[], int cmd_argc);
        void doLogout(std::string cmd_argv[], int cmd_argc);
        void doSync(std::string cmd_argv[], int cmd_argc);
        void doHelp(std::string cmd_argv[], int cmd_argc);
        void doQuit(std::string cmd_argv[], int cmd_argc);
};
#endif