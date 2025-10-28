#include "pop3-v2-cli.h"


POP3V2ClientCLI::POP3V2ClientCLI():CmdLineInterface("pop3-v2-cli>")
{
    this->hostname = "";
    this->user = "";
}

void POP3V2ClientCLI::initCmd() {
    addCmd("login", CLI_CAST(&POP3V2ClientCLI::doLogin));
    addCmd("sync", CLI_CAST(&POP3V2ClientCLI::doSync));
    addCmd("help", CLI_CAST(&POP3V2ClientCLI::doHelp));
    addCmd("quit", CLI_CAST(&POP3V2ClientCLI::doQuit));
}

void POP3V2ClientCLI::doLogin(std::string cmd_argv[], int cmd_argc) {
    // Implementation of login command
}

void POP3V2ClientCLI::doSync(std::string cmd_argv[], int cmd_argc) {
    // Implementation of sync command
}

void POP3V2ClientCLI::doHelp(std::string cmd_argv[], int cmd_argc) {
    console.log("Available commands:\n");
    console.log("  login <host>:<port> <username> <password> - Log in to the POP3v2 server\n");
    console.log("  sync - Synchronize emails\n");
    console.log("  help - Show this help message\n");
    console.log("  quit - Exit the CLI\n");
}

void POP3V2ClientCLI::doQuit(std::string cmd_argv[], int cmd_argc) {
    
}