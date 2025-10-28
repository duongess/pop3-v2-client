#include "utils.h"

void menuClient() {
    console.log("\n=== Client Menu ===\n");
    console.log("1) Text send text\n");
    console.log("2> Email\n");
    console.log("e) End client\n> ");
}

void menuPop3v2() {
    console.log("\n=== Email Menu ===\n");
    console.log("1) Login: login <host>:<port> <username> <password>\n");
    console.log("2) Sync emails: sync\n");
    console.log("q) Quit\n> ");
}

void accountInformation(const std::string& username, const std::string& host, const std::string& port) {
    console.log("\n=== Account Information ===\n");
    console.log("Username: ", username, "\n");
    console.log("Host: ", host, "\n");
    console.log("Port: ", port, "\n");
}
std::string extractValue(const std::string& jsonObject, const std::string& key) {
    // 1. Tìm key (ví dụ: "\"mailId\"")
    std::string keyWithQuotes = "\"" + key + "\"";
    size_t keyPos = jsonObject.find(keyWithQuotes);
    if (keyPos == std::string::npos) return ""; // Không tìm thấy

    // 2. Tìm dấu ':' sau key
    size_t colonPos = jsonObject.find(':', keyPos);
    if (colonPos == std::string::npos) return "";

    // 3. Tìm bắt đầu của giá trị (bỏ qua khoảng trắng)
    size_t valueStart = jsonObject.find_first_not_of(" \t\n", colonPos + 1);
    if (valueStart == std::string::npos) return "";

    // 4. Tìm kết thúc của giá trị (dấu ',' hoặc '}')
    size_t valueEnd = jsonObject.find_first_of(",}", valueStart);
    if (valueEnd == std::string::npos) return "";

    // 5. Lấy chuỗi giá trị (đã loại bỏ khoảng trắng cuối)
    size_t valueLast = jsonObject.find_last_not_of(" \t\n", valueEnd - 1);
    std::string value = jsonObject.substr(valueStart, valueLast - valueStart + 1);
    
    // 6. Nếu là chuỗi (có dấu " "), loại bỏ dấu " "
    if (value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.length() - 2);
    }
    
    return value;
}

/**
 * Hàm chính phân tích toàn bộ chuỗi JSON
 */
std::vector<MailInfo> parseJsonWithoutLibrary(const std::string& jsonString) {
    std::vector<MailInfo> resultVector;

    // 1. Tìm vị trí bắt đầu và kết thúc của mảng "emails"
    size_t arrayStart = jsonString.find("{\n  \"emails\": [\n");
    if (arrayStart == std::string::npos) {
        std::cerr << "Lỗi: Không tìm thấy key 'emails'" << std::endl;
        return resultVector;
    }

    arrayStart = jsonString.find('[', arrayStart);
    if (arrayStart == std::string::npos) {
        std::cerr << "Lỗi: Không tìm thấy mảng '[' sau 'emails'" << std::endl;
        return resultVector;
    }

    size_t arrayEnd = jsonString.find(']', arrayStart);
    if (arrayEnd == std::string::npos) {
        std::cerr << "Lỗi: Không tìm thấy kết thúc mảng ']'" << std::endl;
        return resultVector;
    }

    // 2. Lấy nội dung của mảng (chuỗi chứa các object)
    std::string arrayContent = jsonString.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

    // 3. Lặp qua từng object trong mảng
    size_t currentPos = 0;
    while (true) {
        size_t objStart = arrayContent.find('{', currentPos);
        if (objStart == std::string::npos) {
            break; // Hết object để đọc
        }

        size_t objEnd = arrayContent.find('}', objStart);
        if (objEnd == std::string::npos) {
            break; // Lỗi JSON, không tìm thấy đóng object
        }

        // Lấy chuỗi của 1 object: {"mailId": 1, ...}
        std::string objectStr = arrayContent.substr(objStart, objEnd - objStart + 1);

        // 4. Trích xuất từng giá trị
        try {
            std::string mailIdStr = extractValue(objectStr, "mailId");
            std::string subjectStr = extractValue(objectStr, "subject");
            std::string bodyStr = extractValue(objectStr, "body");

            if (mailIdStr.empty() || subjectStr.empty() || bodyStr.empty()) {
                std::cerr << "Cảnh báo: Thiếu key trong object, bỏ qua..." << std::endl;
                currentPos = objEnd + 1;
                continue; // Bỏ qua object lỗi này
            }

            // 5. Tạo struct MailInfo và gán giá trị
            MailInfo mail;
            mail.mailId = std::stoi(mailIdStr); // Chuyển chuỗi số thành int
            mail.uidl = subjectStr;             // Gán subject -> uidl
            mail.size = bodyStr.length();       // Lấy độ dài body -> size

            // 6. Thêm vào vector kết quả
            resultVector.push_back(mail);

        } catch (const std::exception& e) {
            // Bắt lỗi nếu std::stoi thất bại (ví dụ mailId không phải là số)
            std::cerr << "Lỗi chuyển đổi dữ liệu: " << e.what() << std::endl;
        }

        currentPos = objEnd + 1; // Di chuyển đến sau object vừa đọc
    }

    return resultVector;
}



