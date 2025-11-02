#include "pop3-v2-client.h"

std::string Pop3V2Client::sendPop3V2(std::string mess) {

}

Pop3V2Client::Pop3V2Client():CmdLineInterface("pop3-v2-cli> "),db()
{
    this->hostname = "";
    this->username = "";
    db.initSchema();
    this->initCmd();
    AccountState lastAcc = db.account.getLastAccount();
    if (lastAcc.username != "") {
        this->hostname =  lastAcc.host + ":" + lastAcc.port;
        console.info("[DB] Loaded last account: ", lastAcc.username, "@", this->hostname);
    } else {
        console.info("[DB] No previous account found.");
    }
}

void Pop3V2Client::initCmd() {
    addCmd("login", CLI_CAST(&Pop3V2Client::doLogin));
    addCmd("logout", CLI_CAST(&Pop3V2Client::doLogout));
    addCmd("sync", CLI_CAST(&Pop3V2Client::doSync));
    addCmd("help", CLI_CAST(&Pop3V2Client::doHelp));
    addCmd("quit", CLI_CAST(&Pop3V2Client::doQuit));
}

void Pop3V2Client::doLogin(std::string cmd_argv[], int cmd_argc) {
    if (cmd_argc > 4) {
        console.error("Usage: login <host>:<port> <username> <password>");
        console.log("Run 'help' for more information\n");
        return;
    }
    std::string host, port;
    std::stringstream ss(cmd_argv[1]);
    std::getline(ss, host, ':');
    std::getline(ss, port, ':');

    this->open(host, port);
    if (this->sendStringRequest("USER " + cmd_argv[2] + "\r\n") < 0) {
        this->close();
        return;
    };
    if (this->sendStringRequest("PASS " + cmd_argv[3] + "\r\n") < 0) {
        this->close();
        return;
    }
    this->hostname = cmd_argv[1];
    this->username = cmd_argv[2];
    this->accountId = db.account.setAccount(username, host, port);
    setCmdPrompt(hostname + "@" + username + "> ");
}

void Pop3V2Client::doLogout(std::string cmd_argv[], int cmd_argc) {
    if(this->isConnected()) {
        console.log("Disconnecting...\n");
        this->close();
    } 
    setCmdPrompt("pop3-v2-cli> ");
}

void Pop3V2Client::doSync(std::string cmd_argv[], int cmd_argc) {
    console.log("Synchronizing emails...\n");
    int response = this->sendStringRequest("LIST\r\n");
    if (response < 0) {
        console.error("Failed to retrieve email list.\n");
        return;
    }
    // std::vector<MailInfo> emails = tranferMail(response);
    // for (const auto& email : emails) {
    //     console.log("Email ID: ", email.mailId, "\n");
    //     console.log("Size: ", email.size, "\n");
    // }
    // db.email.saveEmail(accountId, emails);
}

void Pop3V2Client::doHelp(std::string cmd_argv[], int cmd_argc) {
    console.log("Available commands:\n");
    console.log("  login <host>:<port> <username> <password> - Log in to the POP3v2 server\n");
    console.log("  logout - disconnect to the POP3v2 server\n");
    console.log("  sync - Synchronize emails\n");
    console.log("  help - Show this help message\n");
    console.log("  quit - Exit the CLI\n");
}

void Pop3V2Client::doQuit(std::string cmd_argv[], int cmd_argc) {
    this->doLogout(cmd_argv, cmd_argc);
    std::exit(0);
}