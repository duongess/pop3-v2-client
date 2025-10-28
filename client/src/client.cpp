#include "client.h"

Client::Client() : db() {
    db.initSchema();
    AccountState lastAcc = db.account.getLastAccount();
    if (lastAcc.username != "") {
        this->host = lastAcc.host;
        this->port = lastAcc.port;
        console.info("[DB] Loaded last account: ", lastAcc.username, "@", this->host, ":", this->port);
    } else {
        console.info("[DB] No previous account found.");
    }
}

bool Client::connect() {
    if (client.isConnected()) return true;
    if (!client.connectTo(host, port)) {
        console.error("[TCP] Failed to connect to ", host, ":", port);
        return false;
    }
    console.success("[TCP] Connected to ", host, ":", port);
    return true;
}

void Client::disconnect() { client.disconnect(); }

bool Client::isConnected() {
    return client.isConnected();
}

std::string Client::request(const std::string& message) {
    // KHÔNG connectTo ở đây nữa
    if (!client.isConnected()) {
        this->connect();
    }

    if (!client.sendData(message)) {
        console.error("[TCP] Send failed.");
        return "";
    }
    console.info("[TCP] Sent: ", message);

    Response response = client.receiveData(65536);
    if (response.status == Status::OK) {
        console.info("[TCP] Received: ", response.data);
        return response.data;
    }
    console.error("[TCP] Error status: ", (int)response.status, " - ", response.error);
    return "";
}

void Client::sendText(const std::string& message) {
    this->request(message);
};

// helpers
static std::string trimCRLF(std::string s) {
  while (!s.empty() && (s.back()=='\n' || s.back()=='\r')) s.pop_back();
  return s;
}
static bool startsWith(std::string& s, const std::string& pfx) {
  bool sw = s.rfind(pfx, 0) == 0;
  if (!sw) return false;
  s = s.substr(pfx.size() + 1); // thêm kí tự " "
  return true;
}

std::string Client::responsePopv2(const std::string& message) {
  std::string mess = this->request(message);
  mess = trimCRLF(mess);

  if (startsWith(mess, "+OK")) {
    console.success("[POPv2] ", mess);
    return mess;
  }
  if (startsWith(mess, "-ERR")) {
    console.error("[POPv2] ", mess);            
    return "";
  }
  console.warn("[POPv2] Unknown response: ", mess);
  return "";
}
