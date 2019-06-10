


#pragma once

#include <string>
#include <vector>
#include <list>
#include <easysql/sqlexpression.h>

using namespace MysqlUtil;


using ValueContainer = list<pair<Field, shared_ptr<value>>>;
using RECORD = std::vector < std::pair<Field, std::shared_ptr<value>> >;
using RECORDS = std::vector<RECORD>;

int insert(const std::string& sqlstatment, const ValueContainer& values);
int query(const std::string& sqlstatment, const ValueContainer& values, std::vector<Field> columns, RECORDS& records);






