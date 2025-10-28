// Interactive client with menu/cases
#include "client.h"
#include "config/config.h"
#include "utils.h"
#include "pop3-v2-cli.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostClient, port;

  if (argc >= 2) {
    host = argv[1];
  }
  std::filesystem::create_directories("database");
  Client client;
  
  menuClient();
  POP3V2ClientCLI cli;
  cli.initCmd();
  cli.run();

  return 0;
}
