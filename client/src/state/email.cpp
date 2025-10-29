#include "email.h"
bool EmailTable::createTableIfNeeded() {
    // Bật FK (SQLite mặc định OFF theo connection)
    exec_sql(conn_.get(), "PRAGMA foreign_keys = ON;", "PRAGMA_fk");

    static const char* sql =
        "CREATE TABLE IF NOT EXISTS Mail ("
        "  accountId INTEGER NOT NULL,"
        "  mailId    INTEGER NOT NULL,"
        "  size      INTEGER NOT NULL,"
        "  PRIMARY KEY (accountId, mailId),"
        "  FOREIGN KEY (accountId) "
        "    REFERENCES Account(accountId)"
        "    ON UPDATE CASCADE "
        "    ON DELETE CASCADE"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_mail_account ON Mail(accountId);";
    return exec_sql(conn_.get(), sql, "Mail");
}

bool EmailTable::saveEmail(int64_t accountId, const std::vector<MailInfo>& emails) {
    if (emails.empty()) return true;

    char* errMsg = nullptr;
    int rc;

    // Bật FK cho chắc (nếu connection mới)
    rc = sqlite3_exec(conn_.get(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) { sqlite3_free(errMsg); return false; }

    // Transaction
    rc = sqlite3_exec(conn_.get(), "BEGIN IMMEDIATE;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) { sqlite3_free(errMsg); return false; }

    // UPSERT: cập nhật size nếu đã tồn tại
    static const char* SQL =
        "INSERT INTO Mail (accountId, mailId, size) "
        "VALUES (?, ?, ?) "
        "ON CONFLICT(accountId, mailId) DO UPDATE SET "
        "  size = excluded.size;";

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), SQL, -1, &st, nullptr) != SQLITE_OK) {
        sqlite3_exec(conn_.get(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (const auto& m : emails) {
        sqlite3_reset(st);
        sqlite3_clear_bindings(st);

        sqlite3_bind_int64(st, 1, accountId);
        sqlite3_bind_int64(st, 2, static_cast<long long>(m.mailId));
        sqlite3_bind_int64(st, 3, static_cast<long long>(m.size));

        rc = sqlite3_step(st);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(st);
            sqlite3_exec(conn_.get(), "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
    }

    sqlite3_finalize(st);

    rc = sqlite3_exec(conn_.get(), "COMMIT;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_exec(conn_.get(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    console.log("Upserted ", emails.size(), " Mail rows for accountId=", accountId, "\n");
    return true;
}

