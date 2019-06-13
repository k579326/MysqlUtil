

#include "sql_error.h"
#include "easysql/sqlcommand.h"
#include "gensql.h"
#include "sqlcore.h"

namespace MysqlUtil
{

SQLInsert::SQLInsert(SQLTable* table) : table_(table)
{
}

void SQLInsert::Reset()
{
	valueContainer_.clear();
	table_ = nullptr;
}

int SQLInsert::Execute()
{
	if (valueContainer_.empty())
		return SQL_ERR_EMPTY;
	if (!table_)
		return SQL_ERR_NO_TABLE;
	AssignmentExp exp(*valueContainer_.begin());
	for (auto it = valueContainer_.begin() + 1; 
		 it != valueContainer_.end(); it++)
	{
		if (it->empty())
		{
			return SQL_ERR_EMPTY;
		}
		exp && *it;
	}
	valueContainer_.clear();

	std::string sqlString = StatementGen::GenInsertSQL(table_->tableName_, exp);

	insert(sqlString, exp.container_);
	// TODO:
	return SQL_OK;
}

void SQLInsert::SetTable(SQLTable* table)
{
	table_ = table;
}




};