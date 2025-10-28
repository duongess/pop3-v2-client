#include "utils.h"

void menuClient() {
    console.log("\n=== Client Menu ===\n");
    console.log("1) Text send text\n");
    console.log("2> Email\n");
    console.log("e) End client\n> ");
}

void menuPop3v2() {
    console.log("\n=== Email Menu ===\n");
    console.log("1) Login: login <host>:<port> <username> <password>\n");
    console.log("2) Sync emails: sync\n");
    console.log("q) Quit\n> ");
}

void accountInformation(const std::string& username, const std::string& host, const std::string& port) {
    console.log("\n=== Account Information ===\n");
    console.log("Username: ", username, "\n");
    console.log("Host: ", host, "\n");
    console.log("Port: ", port, "\n");
}