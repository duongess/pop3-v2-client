#pragma once
#include <string>

// State 

struct AccountState {
    ssize_t accountId;
    std::string username;
    std::string host;
    std::string port;
};

struct EmailState {
    int mailId;
    std::string subject;
    std::string body;
    ssize_t receivedAt;
};