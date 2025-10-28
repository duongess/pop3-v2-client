#include "common/utils.h"
#include "types/db.h"
void menuClient();
void menuPop3v2();
void accountInformation(const std::string& username, const std::string& host, const std::string& port);
std::string extractValue(const std::string& jsonObject, const std::string& key);
std::vector<MailInfo> parseJsonWithoutLibrary(const std::string& jsonString);


