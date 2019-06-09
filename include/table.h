

#pragma once

#include <string>
#include <vector>
#include "field/field.h"
namespace MysqlUtil
{
	

class SQLTable
{
public:
	SQLTable(const std::string& tablename) : tableName_(tablename){	
	}
	virtual ~SQLTable() {}
	const std::string tableName_;
	std::vector<Field> GetFields(){
		return fieldList_;
	}

protected:
	std::vector<Field> fieldList_;
};


};