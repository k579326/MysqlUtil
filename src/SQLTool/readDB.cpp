

#include <algorithm>
#include "readDB.h"


namespace MysqlUtil
{

    static std::string _LowertoUpper(const std::string& lowerStr)
    {
        std::string upperStr;

        for (auto it = lowerStr.begin(); it != lowerStr.end(); it++)
        {
            upperStr.push_back(toupper(*it));
        }

        return upperStr;
    }

    static std::string _splitFieldType(std::string& typeString)
    {
        size_t pos = typeString.find_first_of('(', 0);

        return typeString.substr(0, pos);
    }

	static bool _IsUnsigned(std::string& typeString)
	{
		size_t pos = typeString.find("unsigned", 0);
		return pos == std::string::npos ? false : true;
	}


    reader::reader(const std::string& machine, const std::string& user, const std::string& pwd, short port, const std::string& db)
    {
        MYSQL* ms = mysql_init(NULL);
        if (ms == NULL)
        {
            std::logic_error err("MYSQL init failed!");
            throw err;
        }
        if (mysql_real_connect(ms, machine.c_str(), user.c_str(), pwd.c_str(), db.c_str(), port, NULL, 0) == NULL)
        {
            mysql_close(ms);
            std::logic_error err("connect db failed!");
            throw err;
        }

        m_ptr = std::shared_ptr<MYSQL>(ms, [](MYSQL* ptr)->void { mysql_close(ptr); });
        m_dbName = db;
    }





    int reader::FetchTableListFromDb()
    {
        int ret = mysql_query(m_ptr.get(), "show tables;");
        if (ret != 0)
        {
            return -1;
        }

        if (mysql_field_count(m_ptr.get()) == 0)
        {// empyt()
            return -1;
        }

        MYSQL_RES* res = mysql_store_result(m_ptr.get());
        if (!res)
        {
            return -1;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        while (row)
        {
            m_tableList.push_back(*row);
            row = mysql_fetch_row(res);
        }

        mysql_free_result(res);

        return 0;
    }

    int reader::FetchAllTableInfo()
    {
        auto readTableStructure = [&](const tableName name)->int
        {
            std::string sql;

            sql = "desc " + name + ";";

            int ret = mysql_query(m_ptr.get(), sql.c_str());
            if (ret != 0)
            {
                return -1;
            }

            if (mysql_field_count(m_ptr.get()) == 0)
            {// empyt()
                return -1;
            }

            MYSQL_RES* res = mysql_store_result(m_ptr.get());
            if (!res)
            {
                return -1;
            }

			int _index = 0;
            MYSQL_ROW row = mysql_fetch_row(res);
            while (row)
            {
                std::string typeString = _LowertoUpper(row[1]);

                auto it = g_ft_map.find(_splitFieldType(typeString));
                if (it == g_ft_map.end())
                {
                    ret = -1;
                    break;
                }

				Field fd(it->second, row[0], _index, _IsUnsigned(typeString));
                m_tablesInfoList[name].push_back(std::move(fd));

                row = mysql_fetch_row(res);
				_index++;
            }
            mysql_free_result(res);

            return ret;
        };

        decltype(readTableStructure) funcRet = std::for_each(m_tableList.begin(), m_tableList.end(), readTableStructure);

        return 0;
    }

    int reader::Generate(const std::string& path)
    {
        for (const auto& element : m_tablesInfoList)
        {
            if (GenTableClass(path, element.first, element.second) != 0)
            {
                return -1;
            }
        }

        return 0;
    }


};


