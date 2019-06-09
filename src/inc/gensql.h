
#pragma once

#include <memory>
#include <string>
#include <vector>


#include "field/field.h"
#include "easysql/sqlexpression.h"

namespace MysqlUtil
{

class StatementGen
{
public:

static std::string GenInsertSQL(const std::string& tablename, const AssignmentExp& values);
static std::string GenQuerySQL(const std::string& tablename, const CompareExp& condition, 
							   const std::vector<Field>& fieldlist);

};




};