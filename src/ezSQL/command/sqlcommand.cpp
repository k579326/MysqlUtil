

#include "sql_error.h"
#include "easysql/sqlcommand.h"
#include "gensql.h"

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

	// TODO:
	return SQL_OK;
}

void SQLInsert::SetTable(SQLTable* table)
{
	table_ = table;
}


void SQLQuery::SetTable(SQLTable* table)
{
	table_ = table;
}
void SQLQuery::Reset()
{
	results_.clear();
	table_ = nullptr;
	condition_.clear();
}
void SQLQuery::SetCondition(const CompareExp& ce)
{
	condition_ = ce;
}

int SQLQuery::Execute()
{
	if (!table_){
		return SQL_ERR_NO_TABLE;
	}
	if (condition_.empty()){
		// no condition select a, b from table;
	}
	if (results_.empty()){
		// result not be set will as 'select * from table ...;'
	}
	
	std::string sqlString = StatementGen::GenQuerySQL(table_->tableName_, condition_, results_);

	results_ = table_->GetFields();

	return SQL_OK;
}
RECORDS SQLQuery::GetResult()
{
	return RECORDS();
}














};

