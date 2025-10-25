#pragma once
#include "utils.h"
#include "../types/response.h"
#include "../types/auth.h"

class Protocol {
protected:
    socket_handle_t sock = invalid_socket_handle;
    static std::atomic<bool> stop_flag;

public:
    AuthSession sess;
    Protocol() = default;
    // Cấm copy
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    // Cho phép move (KHAI BÁO ở header)
    Protocol(Protocol&& other) noexcept;
    Protocol& operator=(Protocol&& other) noexcept;

    bool isValid() const;
    virtual ~Protocol();
    socket_handle_t getSocket(){return this->sock;}

    bool connectTo(const std::string& host, const std::string& port);
    bool isConnected();
    bool bindAndListen(const std::string& host, const std::string& port);
    bool acceptClient(Protocol& client);
    void disconnect();
    void clean();

    static void requestStop();
    static bool shouldStop();
    
    bool sendData(const std::string& data);
    Response receiveData(const size_t& size = 1024);
};
