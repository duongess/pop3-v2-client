// Networking utilities implementation
#include "utils.h"

bool net_init() {
#ifdef _WIN32
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  return iResult == 0;
#else
  return true;
#endif
}

addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive) {
    addrinfo hints{};
    hints.ai_family = AF_INET; // IPv4 only; AF_UNSPEC nếu cần IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (passive)
        hints.ai_flags = AI_PASSIVE; // for bind()

    addrinfo* result = nullptr;
    int gai = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if (gai != 0 || !result) {
        std::cerr << "[TCP] getaddrinfo failed: " << gai << "\n";
        // ❌ Đừng cleanup ở đây — giữ Winsock đang mở
        return nullptr;
    }
    return result;
}

std::vector<std::string> splitWs(std::string_view s) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : s) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!cur.empty()) { out.push_back(cur); cur.clear(); }
    } else cur.push_back(c);
  }
  if (!cur.empty()) out.push_back(cur);
  return out;
}

std::string tolowerCopy(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c){ return (char)std::tolower(c); });
  return s;
}

// (Tuỳ chọn) kiểm tra rất sơ bộ host/ip
bool lookLikeHost(const std::string& s) {
  if (s.empty()) return false;
  // Cho phép chữ, số, '.', '-', ':' (IPv6/port nếu bạn hỗ trợ sau)
  return std::all_of(s.begin(), s.end(), [](unsigned char c){
    return std::isalnum(c) || c=='.' || c=='-' || c==':';
  });
}
