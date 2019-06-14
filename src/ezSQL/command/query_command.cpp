

#include "sql_error.h"
#include "easysql/sqlcommand.h"
#include "gensql.h"

#include "sqlcore.h"
#include "mysql-conn-pool.h"


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

	shared_ptr<MYSQL> SQLptr = mysql_pool::GetInstance()->pl_pick();
	if (SQLptr == nullptr)
	{
		return SQL_ERR_DB_INIT;
	}

	int ret = query(SQLptr.get(), sqlString, condition_.container_, results_, records_);
	mysql_pool::GetInstance()->pl_back(std::move(SQLptr));

	if (ret != 0)
	{
		return ret;
	}
	results_ = table_->GetFields();

	return SQL_OK;
}


const RECORDS& SQLQuery::GetResult() const
{
	return records_;
}

};




