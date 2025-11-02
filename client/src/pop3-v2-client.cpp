#include "pop3-v2-client.h"

static bool checkAndTrimPrefix(std::string& s, const std::string& pfx) {
    // 1. Dùng s.find() thay cho rfind(), dễ đọc hơn
    if (s.find(pfx) != 0) {
        return false; // Không khớp, không làm gì cả
    }

    // 2. Khớp! Cắt bỏ prefix
    s = s.substr(pfx.size());

    // 3. *An toàn* cắt bỏ dấu cách ở đầu (nếu có)
    if (!s.empty() && s[0] == ' ') {
        s = s.substr(1);
    }

    return true;
}

std::string Pop3V2Client::getSingleLineResponse(std::string mess) {
    console.debug("client: " + mess);
    if (this->sendStringRequest(mess + "\r\n") < 0) {
        throw SocketException("Failed to send command: " + mess);
    }

    char buff[1024];
    
    // Chỉ đọc 1 dòng
    int len = this->recvGetLine(buff, sizeof(buff) - 1);
    
    if (len <= 0) {
        throw SocketException("Server disconnected or failed to respond.");
    }

    buff[len] = '\0';
    
    std::string response = buff;
    // Xóa \r\n ở cuối (cách làm của bạn đã đúng)
    if (!response.empty() && response.back() == '\n') {
        response.pop_back();
    }
    if (!response.empty() && response.back() == '\r') {
        response.pop_back();
    }

    // Dùng hàm checkAndTrimPrefix an toàn
    if (checkAndTrimPrefix(response, "-ERR")) {
        throw SocketException(response);
    }

    // Dùng hàm checkAndTrimPrefix
    // 'response' SẼ BỊ THAY ĐỔI, chỉ còn lại nội dung
    checkAndTrimPrefix(response, "+OK"); 
    
    console.success("Server: " + response);
    return response; 
}

std::string Pop3V2Client::getMultiLineResponse(std::string mess) {
    // 1. Gửi lệnh
    if (this->sendStringRequest(mess + "\r\n") < 0) {
        this->close();
        return "";
    }

    char buff[1024];
    std::string receive = "";

    // 2. Đọc dòng phản hồi đầu tiên (+OK hoặc -ERR)
    int len = this->recvGetLine(buff, sizeof(buff) - 1);
    if (len <= 0) {
        this->close();
        return "";
    }
    buff[len] = '\0';
    
    std::string firstLine = buff;
    // Nếu dòng đầu là lỗi (-ERR), trả về ngay
    if (firstLine.substr(0, 4) == "-ERR") {
        return firstLine; 
    }

    // 3. Vòng lặp để đọc data cho đến khi gặp "."
    while (true) {
        len = this->recvGetLine(buff, sizeof(buff) - 1);
        if (len <= 0) {
            this->close(); // Lỗi, server ngắt kết nối giữa chừng
            return ""; 
        }
        buff[len] = '\0';

        // Kiểm tra điều kiện dừng: dòng chỉ có dấu "."
        // (Một số server gửi ".\r\n", số khác gửi ".")
        if (strcmp(buff, ".\r\n") == 0 || strcmp(buff, ".") == 0) {
            break; // Kết thúc thành công
        }

        // Nếu dòng data bắt đầu bằng 2 dấu chấm (byte-stuffing)
        // thì bỏ bớt 1 dấu
        if (buff[0] == '.' && buff[1] == '.') {
            receive += (buff + 1); // Nối chuỗi bắt đầu từ ký tự thứ 2
        } else {
            receive += buff; // Nối toàn bộ dòng vào kết quả
        }
    }
    
    return receive; // Trả về toàn bộ nội dung đã nhận
}

Pop3V2Client::Pop3V2Client():CmdLineInterface("pop3-v2-cli> "),db()
{
    this->hostname = "";
    this->username = "";
    db.initSchema();
    this->initCmd();
    AccountState lastAcc = db.account.getLastAccount();
    if (lastAcc.username != "") {
        this->hostname =  lastAcc.host + ":" + lastAcc.port;
        console.info("[DB] Loaded last account: ", lastAcc.username, "@", this->hostname);
    } else {
        console.info("[DB] No previous account found.");
    }
}

void Pop3V2Client::initCmd() {
    addCmd("login", CLI_CAST(&Pop3V2Client::doLogin));
    addCmd("logout", CLI_CAST(&Pop3V2Client::doLogout));
    addCmd("sync", CLI_CAST(&Pop3V2Client::doSync));
    addCmd("help", CLI_CAST(&Pop3V2Client::doHelp));
    addCmd("quit", CLI_CAST(&Pop3V2Client::doQuit));
}

void Pop3V2Client::doLogin(std::string cmd_argv[], int cmd_argc) {
    // Sửa lỗi: Phải kiểm tra chính xác 4 đối số
    if (cmd_argc != 4) {
        console.error("Usage: login <host>:<port> <username> <password>");
        console.log("Run 'help' for more information\n");
        return;
    }

    // Kiểm tra xem đã đăng nhập chưa
    if (this->isConnected()) {
        console.error("Already logged in. Please 'logout' first.");
        return;
    }

    std::string host, port;
    std::stringstream ss(cmd_argv[1]);
    std::getline(ss, host, ':');
    std::getline(ss, port, ':');

    if (port.empty()) {
        console.error("Invalid format: <host>:<port> is required.");
        return;
    }

    // Bắt đầu khối try để bắt các lỗi SocketException
    try {
        // 1. Mở kết nối
        this->open(host, port); 

        // 2. Gửi USER
        this->getSingleLineResponse("USER " + cmd_argv[2]);

        // 3. Gửi PASS
        this->getSingleLineResponse("PASS " + cmd_argv[3]);

        this->hostname = cmd_argv[1];
        this->username = cmd_argv[2];
        this->accountId = db.account.setAccount(username, host, port);
        setCmdPrompt(hostname + "@" + username + "> ");

        console.log("Login successful. Welcome " + this->username + "!\n");

    } catch (SocketException &e) {
        // 5. Bắt tất cả các lỗi có thể xảy ra
        console.error("Login failed: " + std::string(e.what()));
        this->close();
    }
}

void Pop3V2Client::doLogout(std::string cmd_argv[], int cmd_argc) {
    if(this->isConnected()) {
        console.log("Disconnecting...\n");
        this->close();
    } 
    setCmdPrompt("pop3-v2-cli> ");
}

void Pop3V2Client::doSync(std::string cmd_argv[], int cmd_argc) {
    console.log("Synchronizing emails...\n");
    int response = this->sendStringRequest("LIST\r\n");
    if (response < 0) {
        console.error("Failed to retrieve email list.\n");
        return;
    }
    // std::vector<MailInfo> emails = tranferMail(response);
    // for (const auto& email : emails) {
    //     console.log("Email ID: ", email.mailId, "\n");
    //     console.log("Size: ", email.size, "\n");
    // }
    // db.email.saveEmail(accountId, emails);
}

void Pop3V2Client::doHelp(std::string cmd_argv[], int cmd_argc) {
    console.log("Available commands:\n");
    console.log("  login <host>:<port> <username> <password> - Log in to the POP3v2 server\n");
    console.log("  logout - disconnect to the POP3v2 server\n");
    console.log("  sync - Synchronize emails\n");
    console.log("  help - Show this help message\n");
    console.log("  quit - Exit the CLI\n");
}

void Pop3V2Client::doQuit(std::string cmd_argv[], int cmd_argc) {
    this->doLogout(cmd_argv, cmd_argc);
    std::exit(0);
}