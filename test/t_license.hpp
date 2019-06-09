
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
class t_license : public SQLTable
{
private:
    t_license() : SQLTable("t_license"),
		licenseId(ftVarString, "licenseId", 0, false),
		licKeyId(ftVarString, "licKeyId", 1, false),
		issuerSrcId(ftVarString, "issuerSrcId", 2, false),
		ownerSrcId(ftVarString, "ownerSrcId", 3, false),
		createTime(ftDatetime, "createTime", 4, false),
		testVersion(ftLong, "testVersion", 5, false)

	{
		fieldList_.push_back(licenseId);
		fieldList_.push_back(licKeyId);
		fieldList_.push_back(issuerSrcId);
		fieldList_.push_back(ownerSrcId);
		fieldList_.push_back(createTime);
		fieldList_.push_back(testVersion);

    };

public:
	~t_license() {}
	
    static const t_license& GetInstance()
    {
		static t_license Instance;
        return Instance;
    }

    // fields start
	 const Field licenseId;
	 const Field licKeyId;
	 const Field issuerSrcId;
	 const Field ownerSrcId;
	 const Field createTime;
	 const Field testVersion;

    // fields end

};


}; // namespace
