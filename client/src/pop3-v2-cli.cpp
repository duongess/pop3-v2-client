#include "pop3-v2-cli.h"


POP3V2ClientCLI::POP3V2ClientCLI():CmdLineInterface("pop3-v2-cli> ")
{
    this->hostname = "";
    this->user = "";
}

void POP3V2ClientCLI::initCmd() {
    addCmd("login", CLI_CAST(&POP3V2ClientCLI::doLogin));
    addCmd("logout", CLI_CAST(&POP3V2ClientCLI::doLogout));
    addCmd("sync", CLI_CAST(&POP3V2ClientCLI::doSync));
    addCmd("help", CLI_CAST(&POP3V2ClientCLI::doHelp));
    addCmd("quit", CLI_CAST(&POP3V2ClientCLI::doQuit));
}

void POP3V2ClientCLI::doLogin(std::string cmd_argv[], int cmd_argc) {
    if (cmd_argc > 4) {
        console.error("Usage: login <host>:<port> <username> <password>");
        console.log("Run 'help' for more information\n");
        return;
    }
    std::string host, port;
    std::stringstream ss(cmd_argv[1]);
    std::getline(ss, host, ':');
    std::getline(ss, port, ':');

    this->pop3Client.setIp(host, port);
    if (this->pop3Client.responsePopv2("USER " + cmd_argv[2]) == "") {
        this->pop3Client.disconnect();
        return;
    };
    if (this->pop3Client.responsePopv2("PASS " + cmd_argv[3]) == "") {
        this->pop3Client.disconnect();
        return;
    }
    this->hostname = cmd_argv[1];
    this->user = cmd_argv[2];
    setCmdPrompt(hostname + "@" + user + "> ");
}

void POP3V2ClientCLI::doLogout(std::string cmd_argv[], int cmd_argc) {
    if(this->pop3Client.isConnected()) {
        console.log("Disconnecting...\n");
        this->pop3Client.disconnect();
    } 
    setCmdPrompt("pop3-v2-cli> ");
}

void POP3V2ClientCLI::doSync(std::string cmd_argv[], int cmd_argc) {
    // Implementation of sync command
}

void POP3V2ClientCLI::doHelp(std::string cmd_argv[], int cmd_argc) {
    console.log("Available commands:\n");
    console.log("  login <host>:<port> <username> <password> - Log in to the POP3v2 server\n");
    console.log("  logout - disconnect to the POP3v2 server");
    console.log("  sync - Synchronize emails\n");
    console.log("  help - Show this help message\n");
    console.log("  quit - Exit the CLI\n");
}

void POP3V2ClientCLI::doQuit(std::string cmd_argv[], int cmd_argc) {
    this->doLogout(cmd_argv, cmd_argc);
    std::exit(0);
}