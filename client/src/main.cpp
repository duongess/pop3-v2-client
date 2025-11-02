// Interactive client with menu/cases
#include "utils.h"
#include "pop3-v2-client.h"

int main(int argc, char* argv[]) {
  std::filesystem::create_directories("database");
  
  Pop3V2Client cli;
  cli.run();

  return 0;
}
