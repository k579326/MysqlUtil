

#include "sql_error.h"
#include "sqlcore.h"

#define BUF_MAX_LEN 128

class bindResult
{
public:
	bindResult(enum_field_types& t, unsigned long& maxlen, bool& isunsigned) 
		: type_(t), max_length_(maxlen), is_unsigned_(isunsigned)
	{

	}
	enum_field_types& type_;
	unsigned long& max_length_;
	bool isNull_;
	uint8_t buff_[BUF_MAX_LEN];
	unsigned long length_;
	bool& is_unsigned_;
	bool error_;
};


class Binder
{
public:
	Binder(int size) : size_(size)
	{
		mb_ = new MYSQL_BIND[size_];
		space_ = new bindResult*[size_];
		for (int i = 0; i < size_; i++)
		{
			mb_[i].buffer_length = BUF_MAX_LEN;
			bindResult* e = new bindResult(mb_[i].buffer_type, mb_[i].buffer_length, mb_[i].is_unsigned);
			mb_[i].is_null = &e->isNull_;
			mb_[i].length = &e->length_;
			mb_[i].buffer = e->buff_;
			mb_[i].error = &e->error_;		// 不给error指定空间，会导致内存破坏
			space_[i] = e;
		}	
	}

	~Binder(){
		delete[] mb_;
		for (int i = 0; i < size_; i++)
		{
			delete space_[i];
		}
		delete[] space_;
	}

	MYSQL_BIND* get(){ 
		return mb_;
	}

private:
	MYSQL_BIND* mb_;
	int size_;
public:
	bindResult** space_;
};


int insert(MYSQL* mysql, const std::string& sqlstatment, const ValueContainer& values)
{
	int ret = 0;
	if (mysql == nullptr){
		return SQL_ERR_DB_INIT;
	}

	MYSQL_STMT* stmt = mysql_stmt_init(mysql);

	ret = mysql_stmt_prepare(stmt, sqlstatment.c_str(), sqlstatment.size());
	if (ret != 0)
	{
		return -1;
	}

	std::shared_ptr<Binder> bindPtr(new Binder(values.size()));

	int i = 0;
	for (const auto& element : values)
	{
		string value = element.second->toMysqlValue();
		unsigned long length = value.size();

		memcpy(bindPtr->space_[i]->buff_, value.c_str(), value.size());
		bindPtr->space_[i]->type_ = (enum_field_types)element.first.Type();
		bindPtr->space_[i]->max_length_ = length;
		bindPtr->space_[i]->length_ = length;
		bindPtr->space_[i]->isNull_ = false;
		bindPtr->space_[i]->is_unsigned_ = element.first.IsUnsigned();
		i++;
	}

	if (0 != mysql_stmt_bind_param(stmt, bindPtr->get()))
	{
		ret = mysql_stmt_errno(stmt);
		goto _exit;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		ret = mysql_stmt_errno(stmt);
		const char* errdesc = mysql_stmt_error(stmt);
		goto _exit;
	}

_exit:
	mysql_stmt_close(stmt);
	
	return ret;
}
int query(MYSQL* mysql, const std::string& sqlstatment, const ValueContainer& values, std::vector<Field> columns, RECORDS& records)
{
	int ret = 0;
	
	MYSQL_STMT* stmtptr = mysql_stmt_init(mysql);
	
	std::shared_ptr<MYSQL_STMT> stmt(stmtptr, [](MYSQL_STMT* p)->void{ mysql_stmt_close(p); });

	ret = mysql_stmt_prepare(stmt.get(), sqlstatment.c_str(), sqlstatment.size());
	if (ret != 0)
	{
		return -1;
	}

	std::shared_ptr<Binder> bindPtr(new Binder(values.size()));

	int i = 0;
	for (const auto& element : values)
	{
		string value = element.second->toMysqlValue();
		unsigned long length = value.size();

		memcpy(bindPtr->space_[i]->buff_, value.c_str(), value.size());
		bindPtr->space_[i]->type_ = (enum_field_types)element.first.Type();
		bindPtr->space_[i]->max_length_ = length;
		bindPtr->space_[i]->length_ = length;
		bindPtr->space_[i]->isNull_ = false;
		bindPtr->space_[i]->is_unsigned_ = element.first.IsUnsigned();
		i++;
	}

	if (0 != mysql_stmt_bind_param(stmt.get(), bindPtr->get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	if (0 != mysql_stmt_execute(stmt.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}


	bindPtr.reset(new Binder(columns.size()));
	for (int i = 0; i < columns.size(); i++){
		bindPtr->space_[i]->type_ = (enum_field_types)columns[i].Type();
	}


	if (mysql_stmt_bind_result(stmt.get(), bindPtr->get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	if (mysql_stmt_store_result(stmt.get()))
	{
		return mysql_stmt_errno(stmt.get());
	}

	int count;
	count = mysql_stmt_num_rows(stmt.get());
	// count = mysql_stmt_field_count(stmt.get());

	ret = mysql_stmt_fetch(stmt.get());
	while (ret == 0)
	{
		RECORD record;
		for (int j = 0; j < columns.size(); j++)
		{
			std::shared_ptr<value> valueptr = _CreateValuePtr(bindPtr->space_[j]->buff_, 
				bindPtr->space_[j]->length_, columns[j].Type());
			std::pair<Field, std::shared_ptr<value>> p(columns[j], valueptr);
			record.push_back(std::move(p));
		}
		records.push_back(std::move(record));
		ret = mysql_stmt_fetch(stmt.get());
	}
	
	// const char* p = mysql_stmt_error(stmt.get());

	if (records.size() != count)
	{
		records.clear();
		return mysql_stmt_errno(stmt.get());
	}

	return 0;
}














