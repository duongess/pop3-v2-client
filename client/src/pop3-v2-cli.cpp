#include "pop3-v2-cli.h"


POP3V2ClientCLI::POP3V2ClientCLI():CmdLineInterface("pop3-v2-cli> "),db()
{
    this->hostname = "";
    this->username = "";
    db.initSchema();
    AccountState lastAcc = db.account.getLastAccount();
    if (lastAcc.username != "") {
        this->hostname =  lastAcc.host + ":" + lastAcc.port;
        console.info("[DB] Loaded last account: ", lastAcc.username, "@", this->hostname);
    } else {
        console.info("[DB] No previous account found.");
    }
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
    this->username = cmd_argv[2];
    this->accountId = db.account.setAccount(username, host, port);
    setCmdPrompt(hostname + "@" + username + "> ");
}

void POP3V2ClientCLI::doLogout(std::string cmd_argv[], int cmd_argc) {
    if(this->pop3Client.isConnected()) {
        console.log("Disconnecting...\n");
        this->pop3Client.disconnect();
    } 
    setCmdPrompt("pop3-v2-cli> ");
}

void POP3V2ClientCLI::doSync(std::string cmd_argv[], int cmd_argc) {
    console.log("Synchronizing emails...\n");
    std::string response = this->pop3Client.responsePopv2("LIST");
    if (response == "") {
        console.error("Failed to retrieve email list.\n");
        return;
    }
    std::vector<MailInfo> emails = tranferMail(response);
    for (const auto& email : emails) {
        console.log("Email ID: ", email.mailId, "\n");
        console.log("Size: ", email.size, "\n");
    }
    db.email.saveEmail(emails);


}

void POP3V2ClientCLI::doHelp(std::string cmd_argv[], int cmd_argc) {
    console.log("Available commands:\n");
    console.log("  login <host>:<port> <username> <password> - Log in to the POP3v2 server\n");
    console.log("  logout - disconnect to the POP3v2 server\n");
    console.log("  sync - Synchronize emails\n");
    console.log("  help - Show this help message\n");
    console.log("  quit - Exit the CLI\n");
}

void POP3V2ClientCLI::doQuit(std::string cmd_argv[], int cmd_argc) {
    this->doLogout(cmd_argv, cmd_argc);
    std::exit(0);
}