#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <sstream>
#include <utility>
#include "types/color.h"


class Console {
private:
    static std::string currentTime() {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t t = system_clock::to_time_t(now);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

public:
    ~Console() {
        reset();
    }

    static void reset() {
        std::cout << COLOR::RESET;
    }

    template<typename... Args>
    static void log(Args&&... args) {
        print(COLOR::WHITE, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(char* color, Args&&... args) {
        print(color, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(Args&&... args) {
        println(COLOR::BLUE, "INFO", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void success(Args&&... args) {
        println(COLOR::GREEN, "SUCCESS", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void running(Args&&... args) {
        println(COLOR::BRIGHT_BLACK, "RUNNING", std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    static void stopping(Args&&... args) {
        println(COLOR::BRIGHT_BLACK, "STOPPING", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(Args&&... args) {
        println(COLOR::YELLOW, "WARN", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(Args&&... args) {
        println(COLOR::RED, "ERROR", std::forward<Args>(args)...);
    }
    template<typename... Args>
    static void debug(Args&&... args) {
        running("[DEBUG] ", std::forward<Args>(args)...);
    }
private:
    template<typename... Args>
    static void println(const char* color, const std::string& tag, Args&&... args) {
        std::ostringstream msg;
        (msg << ... << args); // fold expression (C++17)
        std::cout << color
                  << "[" << currentTime() << "] "
                  << "[" << tag << "] "
                  << msg.str()
                  << COLOR::RESET << std::endl;
    }

    template<typename... Args>
    static void print(const char* color, Args&&... args) {
        std::ostringstream msg;
        (msg << ... << args); // fold expression (C++17)
        std::cout << color
                  << msg.str()
                  << COLOR::RESET;
    }
};
