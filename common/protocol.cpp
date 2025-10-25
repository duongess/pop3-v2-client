#include "protocol.h"

std::atomic<bool> Protocol::stop_flag{false};

// helper
static bool set_reuseaddr(socket_handle_t s) {
    int opt = 1;
    return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
}

Protocol::~Protocol() {
    disconnect();
}

bool Protocol::isValid() const { return sock != invalid_socket_handle; }

Protocol::Protocol(Protocol&& other) noexcept
  : sock(std::exchange(other.sock, invalid_socket_handle)) {}

Protocol& Protocol::operator=(Protocol&& other) noexcept {
  if (this != &other) {
    disconnect(); // đóng handle cũ (nếu có)
    sock = std::exchange(other.sock, invalid_socket_handle);
  }
  return *this;
}


bool Protocol::connectTo(const std::string& host, const std::string& port) {
    if (!net_init()) {
        std::cerr << "[TCP] Failed to init networking (WSAStartup)\n";
        return 1;
    }

    addrinfo* result = resolveAddress(host, port, false);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == invalid_socket_handle) continue;

        std::cout << "[DEBUG] Trying connect... " << host << ":" << port << "\n";

        int ret = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (ret == 0) {
            std::cout << "[DEBUG] Connected OK\n";
            break;
        } else {
            int err = WSAGetLastError();
            std::cerr << "[DEBUG] Connect failed with code: " << err << "\n";
            closesocket(sock);
            sock = invalid_socket_handle;
        }
    }

    freeaddrinfo(result);

    if (sock == invalid_socket_handle) {
        std::cerr << "[TCP] Unable to connect to server\n";
        WSACleanup();
        return false;
    }

    std::cout << "[TCP] Connected to " << host << ":" << port << "\n";
    return true;
}

bool Protocol::isConnected() {
    return sock != invalid_socket_handle && stop_flag == false;
}

bool Protocol::bindAndListen(const std::string& host, const std::string& port) {
    if (!net_init()) {
        console.error("[TCP] Failed to init networking (WSAStartup)");
        return 1;
    }

    addrinfo* result = resolveAddress(host, port, true);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == invalid_socket_handle) continue;

        set_reuseaddr(sock);
        if (bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;

        closesocket(sock);
        sock = invalid_socket_handle;
    }
    freeaddrinfo(result);

    if (sock == invalid_socket_handle) {
        console.error("[TCP] Failed to bind socket");
        WSACleanup();
        return false;
    }

    if (listen(sock, SOMAXCONN) != 0) {
        console.error("[TCP] listen failed");
        closesocket(sock);
        WSACleanup();
        return false;
    }

    console.success("[TCP] Listening on port ", port);
    stop_flag = false;
    return true;
}

bool Protocol::acceptClient(Protocol& client) {
    if (client.sock != invalid_socket_handle) return true;
    console.debug("accept() on sock=", this->sock);
    socket_handle_t client_sock = accept(this->sock, nullptr, nullptr);
    console.debug("accept() afef sock=", client_sock);
    if (client_sock == invalid_socket_handle) {
        console.debug("[TCP] accept failed: ", WSAGetLastError());
        return false;
    }
    console.success("accept OK, client sock=", client_sock);
    client.sock = client_sock;
    return true;
}



void Protocol::disconnect() {
    closesocket(this->sock);
}

void Protocol::clean() {
    if (this->sock != invalid_socket_handle) {
        shutdown(this->sock, SD_BOTH);
        this->disconnect();
        this->sock = invalid_socket_handle;
        console.stopping("[TCP] Socket closed\n");
    }
    WSACleanup();
}

void Protocol::requestStop() {
    stop_flag = true;
}

bool Protocol::shouldStop() {
    return stop_flag.load();
}

bool Protocol::sendData(const std::string& data) {
    console.debug("sock: ", sock);
    ssize_t sent = send(sock, data.c_str(), data.size(), 0);
    if (sent < 0) {
        std::cerr << "[TCP] Send failed\n";
        return false;
    }

    return true;
}


Response Protocol::receiveData(const size_t& size) {
    std::vector<char> buffer(size);
    console.debug(123);
    int bytesReceived = recv(sock, buffer.data(), (int)buffer.size(), 0);
    console.debug(456);
    if (bytesReceived < 0) {
        int err = WSAGetLastError();
        std::cerr << "[TCP] Receive error, code=" << err << "\n";
        return { Status::ServerError, {} };
    }

    if (bytesReceived == 0) {
        std::cerr << "[TCP] Connection closed by peer\n";
        return { Status::BadRequest, {} };
    }

    buffer[bytesReceived] = '\0';
    std::string data(buffer.data(), bytesReceived);
    return { Status::OK, data };
}