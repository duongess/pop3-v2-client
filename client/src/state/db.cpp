#include "db.h"

void DB::dumpTables(){
  const char* q = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  sqlite3_stmt* st=nullptr;
  if (sqlite3_prepare_v2(this->conn.get(), q, -1, &st, nullptr)==SQLITE_OK){
    while(sqlite3_step(st)==SQLITE_ROW){
      console.debug("[TABLE] ", (const char*)sqlite3_column_text(st,0));
    }
  }
  sqlite3_finalize(st);
}
