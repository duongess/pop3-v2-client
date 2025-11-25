#pragma once
#include "table.h"
#include "dbConnection.h"
#include <string>

class AccountTable : public Table {
public:
    using Table::Table; // kế thừa ctor: AccountTable(DbConnection&)

    bool createTableIfNeeded() override;

    ssize_t setAccount(const std::string& username, const std::string& host, const std::string& port);
    AccountState getLastAccount();
    // [THÊM MỚI] --- Chức năng Smart Login ---
    bool createHostsTableIfNeeded(); // Tạo bảng hosts (ip, hostname)
    void saveHost(const std::string& hostname, const std::string& ip); // Lưu cặp Host-IP
    std::string getHostname(const std::string& ip); // Tra cứu Hostname từ IP
    // ----------------------------------------


};
