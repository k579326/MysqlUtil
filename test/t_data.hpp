
#pragma once

/*
*	Generated By SQLTOOL, Don't Edit This File
*
*/

#include "table.h"
#include "field/field.h"

namespace MysqlUtil
{
// table
class t_data : public SQLTable
{
private:
    t_data() : SQLTable("t_data"),
		dataKeyId(ftVarString, "dataKeyId", 0, false),
		plain(ftBlob, "plain", 1, false),
		cipher(ftBlob, "cipher", 2, false),
		createTime(ftDatetime, "createTime", 3, false),
		testVersion(ftLong, "testVersion", 4, false)

	{
		fieldList_.push_back(dataKeyId);
		fieldList_.push_back(plain);
		fieldList_.push_back(cipher);
		fieldList_.push_back(createTime);
		fieldList_.push_back(testVersion);

    };

public:
	~t_data() {}
	
    static const t_data& GetInstance()
    {
		static t_data Instance;
        return Instance;
    }

    // fields start
	 const Field dataKeyId;
	 const Field plain;
	 const Field cipher;
	 const Field createTime;
	 const Field testVersion;

    // fields end

};


}; // namespace