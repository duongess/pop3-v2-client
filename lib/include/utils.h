#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <memory>
#include <atomic>
#include <csignal>
#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "console.h"
extern const Console console;

#include "types/state.h"
std::string convertToString(std::vector<MailInfo> mails);
std::vector<MailInfo> convertToMails(const std::string& mailString);

bool checkAndTrimPrefix(std::string& s, const std::string& pfx);
bool isIpAddress(const std::string& str); // Kiểm tra chuỗi có phải IP không
std::string resolveHostname(const std::string& hostname); // Phân giải tên miền ra IP
void logDisconnectError(); // In thông báo mất kết nối

#endif // _UTILS_H_
