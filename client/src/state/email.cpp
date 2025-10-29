#include "email.h";
bool EmailTable :: createTableIfNeeded(){
static const char* sql =
        "CREATE TABLE IF NOT EXISTS Mail ("
        "  mailId INTEGER PRIMARY KEY,"
        "  size INTEGER NOT NULL"
        ");";
    return exec_sql(conn_.get(), sql, "Mail");
}
bool EmailTable::saveEmail(std::vector<MailInfo>emails){
char* errMsg = nullptr;

    for (auto& mail : emails) {
        std::string sql = 
            "INSERT INTO Mail (mailId, size) VALUES (" +
            std::to_string(mail.mailId) + ", " +
            std::to_string(mail.size) + ");";
        int rc = sqlite3_exec(conn_.get(), sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "[DB] Lỗi khi lưu email: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
    }
    std::cout << "[DB] Đã lưu " << emails.size() << " email vào bảng Mail.\n";
    return true;
}