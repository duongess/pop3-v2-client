#pragma once
#include "../../types/state.h"
#include "sqlite3.h"
#include "dbconnection.h"

class Table {
public:
    explicit Table(DbConnection& conn) : conn_(conn) {}
    virtual ~Table() = default;

    // Mỗi bảng tự tạo schema của mình
    virtual bool createTableIfNeeded() = 0;

protected:
    // Helper execute SQL có tag log; định nghĩa ở table.cpp
    static bool exec_sql(sqlite3* db, const char* sql, const char* tag = nullptr);

    DbConnection& conn_;
};