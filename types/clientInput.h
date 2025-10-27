#pragma once
#include <string>
#include <string_view>
#include <variant>

enum class CliCmd { LOGIN, SYNC, QUIT, INVALID };

struct LoginArgs { std::string host; std::string port ;std::string user; std::string pass; };
struct SyncArgs  {};
struct QuitArgs  {};

using CliPayload = std::variant<std::monostate, LoginArgs, SyncArgs, QuitArgs>;

struct ParsedCommand {
  CliCmd cmd{CliCmd::INVALID};
  CliPayload payload{};
  std::string error{};          // != "" nếu parse lỗi
  bool ok() const { return cmd != CliCmd::INVALID && error.empty(); }
};