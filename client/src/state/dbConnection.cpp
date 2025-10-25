#include "dbConnection.h"
#include <stdexcept>

DbConnection::DbConnection(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Failed to open DB: " + path);
    }
    sqlite3_exec(db_, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
}

DbConnection::~DbConnection() {
    if (db_) sqlite3_close(db_);
}

DbConnection::DbConnection(DbConnection&& other) noexcept { db_ = other.db_; other.db_ = nullptr; }
DbConnection& DbConnection::operator=(DbConnection&& other) noexcept {
    if (this != &other) { if (db_) sqlite3_close(db_); db_ = other.db_; other.db_ = nullptr; }
    return *this;
}
