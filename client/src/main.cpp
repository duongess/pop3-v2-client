// Interactive client with menu/cases
#include "client.h"
#include "config/config.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostClient, port;

  if (argc >= 2) {
    host = argv[1];
  }

  Client client;
  
  menuClient();
  while (true) {
    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    std::string messages;

    switch (choice.empty() ? '\0' : choice[0]) {
      case '1': {
        port = cfg.tcp.port;
        std::cout << "Send to server " << host << ":" << port << std::endl;
        std::cout << "messages: "; std::getline(std::cin, messages);
        // client.setIp();
        break;
      }
      case '2': {
        port = cfg.pop3V2.port;
        client.setIp(host, port);
        while (true)
        {
          if (client.isConnected()) {
            accountInformation(client.getUsername(), client.getHost(), client.getPort());
          }
          menuPop3v2();
          std::getline(std::cin, messages);
          if (messages == "quit") break;
          client.sendPopv2(messages);
        }
        
        break;
      }
      case 'q':
      case 'Q':
        return 0;
      default:
        if (!choice.empty()) std::cout << "Unknown option: " << choice << std::endl;
        break;
    }
  }

  return 0;
}
