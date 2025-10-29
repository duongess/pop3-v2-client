#pragma once
#include "account.h"
#include "string"
#include "dbConnection.h"
#include "email.h"
class DB {
public:
    explicit DB()
        : conn("database/CLIENT.db"), account(conn),email(conn) {};

    // tiện ích init schema 1 chỗ
    bool initSchema() {
        return account.createTableIfNeeded() && email.createTableIfNeeded();
    }
    
    void dumpTables();


    DbConnection conn; // nếu cần truy cập thô
    AccountTable account;
    EmailTable   email;
};