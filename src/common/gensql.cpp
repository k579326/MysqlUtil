

#include "gensql.h"

namespace MysqlUtil
{
std::string StatementGen::GenInsertSQL(const std::string& tablename, const AssignmentExp& values)
{
    std::string sql = "insert into " + tablename + "(";

    for (const auto& element : values.container_)
    {
        sql += element.first.Name();
        sql += ",";
    }
    sql.erase(sql.end() - 1);

    sql += ") ";
    sql += "values(";
	sql += values.exp_;
	sql += ");";

    return sql;
}

std::string StatementGen::GenQuerySQL(const std::string& tablename, const CompareExp& condition,
									  const std::vector<Field>& fieldlist)
{
    std::string sql = "select ";

    for (const auto& element : fieldlist)
    {
        sql += element.Name();
        sql += ",";
    }
    if (fieldlist.empty())
    {
        sql += "*";
    }
    else
    {
        sql.erase(sql.end() - 1);
    }
    sql += (" from " + tablename);

	if (!condition.empty())
	{
		sql += (" where " + condition.exp_);
	}

    sql += ";";
    return sql;
}
};

