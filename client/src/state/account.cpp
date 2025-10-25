#include "account.h"

bool AccountTable::createTableIfNeeded() {
    static const char* sql =
        "CREATE TABLE IF NOT EXISTS Account ("
        "  username TEXT NOT NULL,"
        "  host TEXT NOT NULL,"
        "  port TEXT NOT NULL,"
        "  lastAccessed INTEGER NOT NULL,"
        "  syncLast INTEGER NOT NULL DEFAULT 0,"
        "  PRIMARY KEY (username, host, port)"
        ");";
    return exec_sql(conn_.get(), sql, "Account");
}

bool AccountTable::createAccount(const std::string& username, const std::string& host, const std::string& port) {
    const char* sql =
        "INSERT INTO Account(username, host, port, lastAccessed, syncLast) "
        "VALUES(?, ?, ?, strftime('%s','now'), 0) "
        "ON CONFLICT(username, host, port) DO UPDATE SET "
        "  lastAccessed = strftime('%s','now');";

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(st, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, host.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 3, port.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(st);
    sqlite3_finalize(st);
    return rc == SQLITE_DONE;
}

AccountState AccountTable::getLastAccount() {
    const char* sql =
        "SELECT username, host, port "
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
        account.username = reinterpret_cast<const char*>(sqlite3_column_text(st, 0));
        account.host = reinterpret_cast<const char*>(sqlite3_column_text(st, 1));
        account.port = reinterpret_cast<const char*>(sqlite3_column_text(st, 2));
    }

    sqlite3_finalize(st);
    return account;
} 