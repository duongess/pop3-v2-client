#include "table.h"


bool Table::exec_sql(sqlite3* db, const char* sql, const char* tag) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
    if (rc != SQLITE_OK) {
        // in ra tag + rc + errmsg để biết chết ở đâu
        if (tag) console.error("[SQL][", tag, "] rc=", rc, " msg=", (err ? err : "null"));
        else     console.error("[SQL] rc=", rc, " msg=", (err ? err : "null"));
        if (err) sqlite3_free(err);
        return false;
    }
    return true;
}
