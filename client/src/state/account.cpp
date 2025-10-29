#include "account.h"

bool AccountTable::createTableIfNeeded() {
    static const char* sql =
        "CREATE TABLE IF NOT EXISTS Account ("
        "  accountId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT NOT NULL,"
        "  host TEXT NOT NULL,"
        "  port TEXT NOT NULL,"
        "  lastAccessed INTEGER NOT NULL,"
        "  syncLast INTEGER NOT NULL DEFAULT 0,"
        "  unique (username, host, port)"
        ");";
    return exec_sql(conn_.get(), sql, "Account");
}

 ssize_t AccountTable::setAccount(const std::string& username, const std::string& host, const std::string& port) {
    static const char* SQL =
        "INSERT INTO Account(username, host, port, lastAccessed, syncLast) "
        "VALUES(?, ?, ?, strftime('%s','now'), 0) "
        "ON CONFLICT(username, host, port) DO UPDATE SET "
        "  lastAccessed = excluded.lastAccessed "
        "RETURNING accountId;";  // <-- quan trọng

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), SQL, -1, &st, nullptr) != SQLITE_OK) {
        return 0;
    }

    // Bind params
    sqlite3_bind_text(st, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, host.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 3, port.c_str(), -1, SQLITE_TRANSIENT);

    ssize_t rc = sqlite3_step(st);
    ssize_t result;
    if (rc == SQLITE_ROW) {
        result = sqlite3_column_int64(st, 0);
    }
    sqlite3_finalize(st);
    return result;
}

AccountState AccountTable::getLastAccount() {
    const char* sql =
        "SELECT accountId, username, host, port "
        "FROM Account "
        "ORDER BY lastAccessed DESC "
        "LIMIT 1;";

    sqlite3_stmt* st = nullptr;
    AccountState account{};
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr) != SQLITE_OK) {
        return account; // return empty account on failure
    }

    int rc = sqlite3_step(st);
    if (rc == SQLITE_ROW) {
        account.accountId = reinterpret_cast<const ssize_t>(sqlite3_column_text(st, 0));
        account.username = reinterpret_cast<const char*>(sqlite3_column_text(st, 1));
        account.host = reinterpret_cast<const char*>(sqlite3_column_text(st, 2));
        account.port = reinterpret_cast<const char*>(sqlite3_column_text(st, 3));
    }

    sqlite3_finalize(st);
    return account;
} 