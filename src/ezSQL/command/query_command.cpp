

#include "sql_error.h"
#include "easysql/sqlcommand.h"
#include "gensql.h"





namespace MysqlUtil
{



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




