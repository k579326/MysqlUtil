

#include "readDB.h"
#include <memory>
#include <iostream>
#include "tinyxml.h"
#include "dbconfig.h"

using namespace std;
using namespace MysqlUtil;


int main()
{
	db_config_t config;

	int ret = load_config(config);
	if (ret != 0)
	{
		return ret;
	}

    try
    {
        reader r(config.host, config.user, config.pwd, config.port, config.dbName);
        r.FetchTableListFromDb();
        r.FetchAllTableInfo();
        if (0 != r.Generate("./config"))
        {
            cout << "gen failed!" << endl;
            return -1;
        }

        cout << "success!" << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
