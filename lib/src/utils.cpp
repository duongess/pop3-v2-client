#include "utils.h"

std::string convertToString(std::vector<MailInfo> mails){
    std::string cur = "";
    for (MailInfo mail:mails) {
        cur += std::to_string(mail.mailId) + " " + std::to_string(mail.size) + "\r\n";
    }
    cur += "\r\n";
    console.debug(cur);
    return cur;
}