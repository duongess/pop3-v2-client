#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>

// ===== Platform detection =====
#if defined(_WIN32) || defined(_WIN64)
  #define NET_PLATFORM_WINDOWS 1
#else
  #define NET_PLATFORM_WINDOWS 0
#endif

// ===== Platform includes & types =====
#if NET_PLATFORM_WINDOWS
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
  using socket_handle_t = SOCKET;
  constexpr socket_handle_t invalid_socket_handle = INVALID_SOCKET;
  #define NET_SOCKET_ERROR SOCKET_ERROR
  // For Windows linking: ws2_32.lib
#else
  // POSIX
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <netinet/in.h>
  #include <unistd.h>
  using socket_handle_t = int;
  constexpr socket_handle_t invalid_socket_handle = -1;
  #define NET_SOCKET_ERROR (-1)
  #define closesocket close
  #define WSAGetLastError() errno
  #define WSACleanup() (void)0
  #define SD_BOTH SHUT_RDWR
#endif

// Nếu bạn có console riêng thì giữ lại include; nếu không, bỏ dòng dưới:
#include "console.h"
extern const Console console;

// ===== API =====
bool net_init();

// Trả về IP LAN nếu host là localhost/127.0.0.1/::1, ngược lại trả về host gốc
std::string normalizeHostForLAN(std::string host);

// Cấp phát addrinfo (nhớ free bằng freeaddrinfo)
addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive);

// Helpers
std::vector<std::string> splitWs(std::string_view s);
std::string tolowerCopy(std::string s);

// Heuristic xem chuỗi có giống “host” (domain/ip) không
bool lookLikeHost(const std::string& s);

