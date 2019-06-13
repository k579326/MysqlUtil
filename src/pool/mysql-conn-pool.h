
#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include "conn-pool.h"
#include "dbconfig.h"

using namespace pool;

class mysql_pool : public common_pool<MYSQL>
{
public:

    static mysql_pool* GetInstance()
    {
        return &m_instance;
    }

private:
    
    mysql_pool(int pool_size) : common_pool<MYSQL>(pool_size) {
        m_instance.init();
    }
    ~mysql_pool() {
		m_instance.uninit();
    }
private:
    virtual shared_ptr<MYSQL> createRes() override
    {
		db_config_t config;
        MYSQL* ms = mysql_init(NULL);
        if (ms == NULL)
        {
            return nullptr;
        }
		shared_ptr<MYSQL> mysqlPtr(ms, [](MYSQL* ms)->void { mysql_close(ms); });

		if (load_config(config) != 0)
		{
			return nullptr;
		}

        if (mysql_real_connect(ms, config.host.c_str(), config.user.c_str(), config.pwd.c_str(), 
							   config.dbName.c_str(), config.port, NULL, 0) == NULL)
        {
            return nullptr;
        }

        return mysqlPtr;
    }

private:
    static mysql_pool m_instance;
};
