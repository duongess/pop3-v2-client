#include "utils.h"

void menuClient() {
    console.log("\n=== Client Menu ===\n");
    console.log("1) Text send text\n");
    console.log("2> Email\n");
    console.log("e) End client\n> ");
}

void menuPop3v2() {
    console.log("\n=== Email Menu ===\n");
    console.log("1) Login: login <host> <username> <password>\n");
    console.log("2) Sync emails: sync\n");
    console.log("q) Quit\n> ");
}

void accountInformation(const std::string& username, const std::string& host, const std::string& port) {
    console.log("\n=== Account Information ===\n");
    console.log("Username: ", username, "\n");
    console.log("Host: ", host, "\n");
    console.log("Port: ", port, "\n");
}

// --- parser chính ---
ParsedCommand parseCliLine(std::string_view line) {
  ParsedCommand pc;
  auto t = splitWs(line);
  if (t.empty()) { pc.error = "Empty command"; return pc; }

  const std::string cmd = tolowerCopy(t[0]);
  if (cmd == "login") {
    if (t.size() != 4) { pc.error = "Usage: login <host> <username> <password>"; return pc; }
    if (!lookLikeHost(t[1])) { pc.error = "Invalid host/IP"; return pc; }
    pc.cmd = CliCmd::LOGIN;
    pc.payload = LoginArgs{ t[1], t[2], t[3] };
    return pc;
  }

  if (cmd == "sync") {
    if (t.size() != 1) { pc.error = "Usage: sync"; return pc; }
    pc.cmd = CliCmd::SYNC;
    pc.payload = SyncArgs{};
    return pc;
  }

  if (cmd == "quit") {
    if (t.size() != 1) { pc.error = "Usage: quit"; return pc; }
    pc.cmd = CliCmd::QUIT;
    pc.payload = QuitArgs{};
    return pc;
  }

  pc.error = "Unknown command: " + t[0];
  return pc;
}