// Interactive client with menu/cases
#include "client.h"
#include "utils.h"
#include "pop3-v2-client.h"

int main(int argc, char* argv[]) {
  std::filesystem::create_directories("database");
  
  Pop3V2Client cli;
  cli.initCmd();
  cli.run();

  return 0;
}
