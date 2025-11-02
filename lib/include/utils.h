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

#include "console.h"
extern const Console console;

#include "types/state.h"
std::string convertToString(std::vector<MailInfo> mails);