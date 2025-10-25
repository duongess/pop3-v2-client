#pragma once
#include "table.h"

class AccountTable : public Table {
public:
    using Table::Table; // kế thừa ctor: AccountTable(DbConnection&)

    bool createTableIfNeeded() override;

    bool createAccount(const std::string& username, const std::string& host, const std::string& port);
    AccountState getLastAccount();
};
