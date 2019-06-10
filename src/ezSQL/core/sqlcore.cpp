


#include "sqlcore.h"
#include "mysql-conn-pool.h"


class bindResult
{
public:

	bool isNull;
	uint8_t buff[128];
	unsigned long length;
};



int insert(const std::string& sqlstatment, const ValueContainer& values)
{
	int ret = 0;
	std::shared_ptr<MYSQL> sqlPtr = mysql_pool::GetInstance()->pl_pick();

	MYSQL_STMT* stmt = mysql_stmt_init(sqlPtr.get());

	ret = mysql_stmt_prepare(stmt, sqlstatment.c_str(), sqlstatment.size());
	if (ret != 0)
	{
		return -1;
	}

	std::shared_ptr<MYSQL_BIND> bindPtr(new MYSQL_BIND[values.size()], [](MYSQL_BIND* p)->void { delete[] p; });

	int i = 0;
	for (const auto& element : values)
	{
		string value = element.second->toMysqlValue();
		unsigned long length = value.size();
		bindPtr.get()[i].buffer_type = (enum_field_types)element.first.Type();
		bindPtr.get()[i].buffer = (void*)value.c_str();
		bindPtr.get()[i].is_null = (bool*)0;
		bindPtr.get()[i].buffer_length = length;
		bindPtr.get()[i].length = &length;
		bindPtr.get()[i].is_unsigned = element.first.IsUnsigned();
		i++;
	}

	if (!mysql_stmt_bind_param(stmt, bindPtr.get()))
	{
		ret = -1;
		goto _exit;
	}

	ret = mysql_stmt_execute(stmt);
	if (ret != 0)
	{
		goto _exit;
	}

_exit:
	mysql_stmt_close(stmt);

	return ret;
}
int query(const std::string& sqlstatment, const ValueContainer& values, std::vector<Field> columns, RECORDS& records)
{
	int ret = 0;
	std::shared_ptr<MYSQL> sqlPtr = mysql_pool::GetInstance()->pl_pick();

	MYSQL_STMT* stmtptr = mysql_stmt_init(sqlPtr.get());

	std::shared_ptr<MYSQL_STMT> stmt(stmtptr, [](MYSQL_STMT* p)->void{ mysql_stmt_close(p); });

	ret = mysql_stmt_prepare(stmt.get(), sqlstatment.c_str(), sqlstatment.size());
	if (ret != 0)
	{
		return -1;
	}

	std::shared_ptr<MYSQL_BIND> bindPtr(new MYSQL_BIND[values.size()], [](MYSQL_BIND* p)->void { delete[] p; });

	int i = 0;
	for (const auto& element : values)
	{
		string value = element.second->toString();
		unsigned long length = value.size();
		bindPtr.get()[i].buffer_type = (enum_field_types)element.first.Type();
		bindPtr.get()[i].buffer = (void*)value.c_str();
		bindPtr.get()[i].is_null = (bool*)0;
		bindPtr.get()[i].buffer_length = value.size();
		bindPtr.get()[i].length = &length;
		bindPtr.get()[i].is_unsigned = element.first.IsUnsigned();
		i++;
	}

	if (!mysql_stmt_bind_param(stmt.get(), bindPtr.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	if (mysql_stmt_execute(stmt.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}


	bindPtr.reset(new MYSQL_BIND[columns.size()], [](MYSQL_BIND* p)->void { delete[] p; });

	if (mysql_stmt_bind_result(stmt.get(), bindPtr.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	if (mysql_stmt_store_result(stmt.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	int count;
	count = mysql_stmt_num_rows(stmt.get());
	count = mysql_stmt_field_count(stmt.get());

	std::shared_ptr<bindResult> mem(new bindResult[columns.size()], [](bindResult* p)->void { delete[] p; });
	while (mysql_stmt_fetch(stmt.get()) == 0)
	{
		RECORD record;
		for (int j = 0; j < columns.size(); j++)
		{
			bindPtr.get()[j].buffer_type = (enum_field_types)columns[j].Type();
			bindPtr.get()[j].buffer = mem.get()[j].buff;
			bindPtr.get()[j].is_null = &mem.get()[j].isNull;
			bindPtr.get()[j].buffer_length = sizeof(mem.get()[j].buff);
			bindPtr.get()[j].length = &mem.get()[j].length;

			std::shared_ptr<value> valueptr = _CreateValuePtr(mem.get()[j].buff, mem.get()[j].length, columns[j].Type());
			std::pair<Field, std::shared_ptr<value>> p(columns[j], valueptr);
			record.push_back(std::move(p));
		}
		records.push_back(std::move(record));
	}
	
	if (records.size() != count)
	{
		records.clear();
		return mysql_stmt_errno(stmt.get());
	}

	return 0;
}














