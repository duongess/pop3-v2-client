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

std::vector<MailInfo> convertToMails(const std::string& mailString) {
    std::vector<MailInfo> result;
    std::istringstream iss(mailString);
    int id, size;
    while (iss >> id >> size) {
        MailInfo mail;
        mail.mailId = id;
        mail.size = size;
        result.push_back(mail);
    }
    return result;
}

bool checkAndTrimPrefix(std::string& s, const std::string& pfx) {
    if (s.find(pfx) != 0) {
        return false;
    }

    s = s.substr(pfx.size());

    if (!s.empty() && s[0] == ' ') {
        s = s.substr(1);
    }

    return true;
}