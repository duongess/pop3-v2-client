#pragma once
#include "sqlite3.h"
#include "../../common/utils.h"

class DbConnection {
public:
    explicit DbConnection(const std::string& path);
    ~DbConnection();

    // non-copyable but movable
    DbConnection(const DbConnection&) = delete;
    DbConnection& operator=(const DbConnection&) = delete;
    DbConnection(DbConnection&&) noexcept;
    DbConnection& operator=(DbConnection&&) noexcept;

    sqlite3* get() const { return db_; }

private:
    sqlite3* db_ = nullptr;
};
