#pragma once
#include "table.h"
#include "types/db.h"
class EmailTable: public Table{
public:
using Table::Table;
bool createTableIfNeeded() override;
bool saveEmail(std::vector<MailInfo>emails);

};
