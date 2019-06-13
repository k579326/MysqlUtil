
#pragma once

#include <vector>
#include "field/field.h"
#include "easysql/sqlexpression.h"
#include "table.h"

namespace MysqlUtil
{

class SQLCommand
{
public:
	SQLCommand() {}
	virtual ~SQLCommand() {}
	
	virtual void SetTable(SQLTable* table) = 0;
	virtual int Execute() = 0;
	virtual void Reset() = 0;
protected:
	template<typename T>
	void ExpandArgs(std::vector<T>& container, const T& element)
	{
		container.push_back(element);
	}
	
	template<typename T, typename... ARGS>
	void ExpandArgs(std::vector<T>& container, const T& element, ARGS... argList)
	{
		container.push_back(element);
		ExpandArgs(container, argList...);
	}

};



class SQLInsert : public SQLCommand
{
public:
	SQLInsert(SQLTable* table);
	~SQLInsert() {}
	
	template<typename... ARGS>
	void SetValues(ARGS... fs)
	{
		ExpandArgs(valueContainer_, fs...);
	}
	
	int Execute() override;
	void Reset() override;
	void SetTable(SQLTable* table) override;
private:
	std::vector<AssignmentExp> valueContainer_;
	SQLTable* table_;	
};





/*  query Command */ 
using RECORD = std::vector < std::pair<Field, std::shared_ptr<value>> >;
using RECORDS = std::vector<RECORD>;

class SQLQuery : public SQLCommand
{
	
public:
	SQLQuery(SQLTable* table) : table_(table) {}
	virtual ~SQLQuery() {}
	
	template<typename... ARGS>
	void SetResultFields(ARGS... fields)
	{
		ExpandArgs(results_, fields...);
	}
	void SetCondition(const CompareExp& ce);
	void SetTable(SQLTable* table) override;
	void Reset() override;
	int Execute() override;
	const RECORDS& GetResult() const;

private:
	RECORDS records_;
	CompareExp condition_;
	std::vector<Field> results_;
	SQLTable* table_;
};





	
};


