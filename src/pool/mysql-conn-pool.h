
#pragma once

#include <mysql/mysql.h>
#include "conn-pool.h"

using namespace pool;

class mysql_pool : public common_pool<MYSQL>
{
public:

    static mysql_pool* GetInstance()
    {
		m_instance.init();
        return &m_instance;
    }

    static bool DestoryInstance()
    {
		m_instance.uninit();
        return true;
    }
private:
    
    mysql_pool(int pool_size) : common_pool<MYSQL>(pool_size) {}

private:
    virtual shared_ptr<MYSQL> createRes() override
    {
        MYSQL* ms = mysql_init(NULL);
        if (ms == NULL)
        {
            return nullptr;
        }
        if (mysql_real_connect(ms, "localhost", "K", "123", "DOE_compatibility_test_db", 3306, NULL, 0) == NULL)
        {
            mysql_close(ms);
            return nullptr;
        }
        return shared_ptr<MYSQL>(ms, [](MYSQL* ms)->void{ mysql_close(ms); });
    }

private:
    static mysql_pool m_instance;
};
