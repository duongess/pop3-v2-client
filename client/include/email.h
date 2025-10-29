#pragma once
#include "table.h"
#include "types/db.h"
class EmailTable: public Table{
public:
using Table::Table;
bool createTableIfNeeded() override;
bool saveEmail(int64_t accountId, const std::vector<MailInfo>& emails);

};
