#pragma once


#include <exception>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <mysql/mysql.h>
#include "field/field.h"
#include "genClass/gen-tableclass.h"

namespace MysqlUtil
{

class reader
{
public:

    reader(const std::string& machine, const std::string& user, const std::string& pwd, short port, const std::string& db);

    ~reader() { m_ptr.reset((MYSQL*)nullptr); }

    reader(const reader& r) = delete;
    reader(reader&& r) = delete;
    reader& operator= (const reader& r) = delete;


    int FetchTableListFromDb();
    int FetchAllTableInfo();
    int Generate(const std::string& path);



private:

    typedef std::string tableName;

    std::vector<tableName> m_tableList;
    std::map<tableName, std::vector<Field>> m_tablesInfoList;

    std::string m_dbName;
    std::shared_ptr<MYSQL> m_ptr;
};


};


