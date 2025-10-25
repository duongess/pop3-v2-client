#include "common/utils.h"
#include "types/clientInput.h"

void menuClient();
void menuPop3v2();
void accountInformation(const std::string& username, const std::string& host, const std::string& port);
ParsedCommand parseCliLine(std::string_view line);