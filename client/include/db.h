#pragma once
#include "account.h"
#include "string"
#include "dbConnection.h"

class DB {
public:
    explicit DB()
        : conn("database/CLIENT.db"), account(conn) {}

    // tiện ích init schema 1 chỗ
    bool initSchema() {
        return account.createTableIfNeeded();
    }
    
    void dumpTables();


    DbConnection conn; // nếu cần truy cập thô
    AccountTable account;
    // MailTable mail;
};