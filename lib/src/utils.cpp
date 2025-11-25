#include "utils.h"
#include <iostream>
#include "console.h"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    // Thư viện cho Linux/Mac
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

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

// 1. Kiểm tra sơ bộ xem có phải IP không
bool isIpAddress(const std::string& str) {
    // Cách đơn giản: IP chỉ chứa số và dấu chấm
    return str.find_first_not_of("0123456789.") == std::string::npos;
}

// 2. Phân giải Hostname -> IP
std::string resolveHostname(const std::string& hostname) {
    struct addrinfo hints = {}, *res = nullptr;
    hints.ai_family = AF_INET; // Chỉ lấy IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &res) != 0) {
        return ""; // Lỗi phân giải
    }

    char ipStr[INET_ADDRSTRLEN];
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);

    freeaddrinfo(res);
    return std::string(ipStr);
}

// 3. Hàm Log ngắt kết nối (Sử dụng console logger của bạn)
void logDisconnectError() {
    // In ra màn hình thật nổi bật
    console.error("\n");
    console.error("==========================================");
    console.error(" ⚠️  CONNECTION LOST / SERVER DISCONNECTED ");
    console.error("    (Mat ket noi voi may chu)             ");
    console.error("==========================================");
    console.error("\n");
}