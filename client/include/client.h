#include "common/utils.h"
#include "utils.h"
#include "common/protocol.h"
#include "types/request.h"
#include "types/clientInput.h"
#include "types/state.h"
#include "db.h"

using pop::RequestPopV2;

class Client {
    private:
        std::string username;
        std::string host;
        std::string port;
        std::string token;
        Protocol client;
        DB db;
        std::string request(const std::string& message);
        bool connect();
        void close();
    public:
        Client();
        void setIp(const std::string& host, const std::string& port) {
            this->host = host;
            this->port = port;
        }

        bool isConnected();
        std::string getUsername() const { return this->username; }
        std::string getHost() const { return this->host; }
        std::string getPort() const { return this->port; }

        void sendText(const std::string& message);
        void sendPopv2(const std::string& message);

        std::string responsePopv2(const std::string& message);
};